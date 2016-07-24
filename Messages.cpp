#include <string>
#include <vector>
#include "Messages.h"
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"

TS3Interface::TS3Interface()
{
	printf("TS3Interface Initialized with no functions\n");
	this->groupList = new std::vector<anyID>();
	this->clientList = new std::vector<Client>();
}

TS3Interface::TS3Interface(TS3Functions func)
{
	this->interfaceFunctions = func;
}

TS3Interface::~TS3Interface()
{
	if (groupList)
	{
		groupList->clear();
		delete groupList;
		groupList = NULL;
	}

	clientList->empty();
	delete clientList;
	clientList = NULL;
}

void TS3Interface::updateServerConnection()
{
	this->serverConnectionHandle = this->interfaceFunctions.getCurrentServerConnectionHandlerID();
}

void TS3Interface::setServerConnection(uint64 server)
{
	if (server) this->serverConnectionHandle = server;
}

void TS3Interface::setFunctions(TS3Functions func)
{
	this->interfaceFunctions = func;
}

void TS3Interface::printToTab(std::string message)
{
	if (!serverConnectionHandle)
		return;
	interfaceFunctions.printMessageToCurrentTab(message.c_str());
}

bool TS3Interface::sendChannelMessage(std::string message)
{
	if (!serverConnectionHandle)
		return false;

	uint64 channelID = NULL;
	anyID clientID[128];

	if (interfaceFunctions.getClientID(serverConnectionHandle, clientID) == ERROR_ok)
		if (interfaceFunctions.getChannelOfClient(serverConnectionHandle, clientID[0], &channelID) == ERROR_ok)
			if (interfaceFunctions.requestSendChannelTextMsg(serverConnectionHandle, message.c_str(), channelID, NULL) == ERROR_ok)
				return true;
	return false;
}

bool TS3Interface::sendPoke(anyID clientID, std::string message)
{
	if (!serverConnectionHandle)
		return false;

	return interfaceFunctions.requestClientPoke(serverConnectionHandle, clientID, message.c_str(), NULL) == ERROR_ok;
}

bool TS3Interface::kickClient(anyID clientID)
{
	if (!serverConnectionHandle)
		return false;

	return interfaceFunctions.requestClientKickFromServer(serverConnectionHandle, clientID, "As requested.", NULL) == ERROR_ok;
}

bool TS3Interface::sendPrivateMessage(std::string message, anyID clientID)
{
	if (!serverConnectionHandle)
		return false;

	return interfaceFunctions.requestSendPrivateTextMsg(serverConnectionHandle, message.c_str(), clientID, NULL) == ERROR_ok;
}

std::string TS3Interface::getClientName(anyID clientID)
{
	if (!serverConnectionHandle)
		return "";

	char *name = new char[TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK];
	std::string clientName;

	if (interfaceFunctions.getClientDisplayName(serverConnectionHandle, clientID, name, TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK) == ERROR_ok)
	{
		clientName = std::string(name);
		printf("Client name is %s\n", name);
		interfaceFunctions.freeMemory(name);
	}
	else
		printf("Could not get client name!\n");

	return clientName;
}

std::string TS3Interface::getClientUID(anyID clientID)
{
	if (!serverConnectionHandle)
		return "";

	char *clientUID;
	std::string UID;

	if (interfaceFunctions.getClientVariableAsString(serverConnectionHandle, clientID, CLIENT_UNIQUE_IDENTIFIER, &clientUID) == ERROR_ok)
	{
		UID = std::string(clientUID);
		interfaceFunctions.freeMemory(clientUID);
		return UID;
	}

	return "";
}

bool TS3Interface::getGroupClientList(uint64 groupID)
{
	return interfaceFunctions.requestServerGroupClientList(serverConnectionHandle, groupID, 1, NULL) == ERROR_ok;
}

void TS3Interface::addClientToGroupList(std::string UID)
{
	printf("Recieved UID ->%s\n", UID.c_str());
	this->groupClientList.push_back(UID);
}

std::vector<anyID> *TS3Interface::retrieveRequestedList()
{
	groupList->clear();
	printf("Connected clients ->%i\n", clientList->size());
	printf("Group Client List ->%i\n", groupClientList.size());

	for (int i = 0; i < clientList->size(); i++)
	{
		for (int j = 0; j < groupClientList.size(); j++)
		{
			//printf("Comparing %s vs %s\n", groupClientList[j].c_str(), allClients[i].UID().c_str());
			if (clientList->at(i).UID().compare(groupClientList[j]) == 0)
			{
				printf("==========Matched UID to UID in Group=======\n");
				groupList->push_back(clientList->at(i).clientID());
				break;
			}
		}
	}

	return this->groupList;
}
void TS3Interface::subscribeAllChannels()
{
	interfaceFunctions.requestChannelSubscribeAll(serverConnectionHandle, NULL);
}

std::vector<Client> *TS3Interface::getConnectedClients()
{
	anyID *idList;
	clientList->clear();
	
	if (interfaceFunctions.getClientList(serverConnectionHandle, &idList) == ERROR_ok)
	{
		for (int i = 0; idList[i] != NULL; i++)
		{
			printf("Added client with cID[%i] to list\n", idList[i]);
			std::string cName = getClientName(idList[i]);
			std::string cUID = getClientUID(idList[i]);
			clientList->push_back(Client(cName, cUID, idList[i]));
		}
		interfaceFunctions.freeMemory(clientList);
	}

	return clientList;
}

Client *TS3Interface::getClientByID(anyID cID)
{
	for (int i = 0; i < clientList->size(); i++)
		if (clientList->at(i).clientID() == cID)
			return &clientList->at(i);
	return NULL;
}

Client *TS3Interface::getClientByName(std::string name)
{
	printf("Searching for client (%i) total.\n", clientList->size());
	for (int i = 0; i < clientList->size(); i++)
	{
		//printf("Comparing \"%s\" vs \"%s\"", name, clientList->at(i).Name().c_str());
		if (clientList->at(i).Name().compare(name) == 0)
			return &clientList->at(i);
	}
	return NULL;
}
