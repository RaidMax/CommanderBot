#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <thread>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "include/ts3_functions.h"
#include "Main.h"
#include "_Main.h"
#include "EventPull.h"
#include "Poco/URI.h"
#include "CommandManager.h"
#include "Messages.h"
#include "Console.h"


static char *pluginID = NULL;
static std::vector<std::string> adminList;
std::vector<anyID> onlineAdminList;
bool muted = false;
anyID hostID;

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#endif

/*
std::string getClientIP(anyID clientID)
{
	std::string strIP;
	if (clientID == hostID)
	{
		if (ts3Functions.getClientSelfVariableAsString(serverConnectionHandle, CONNECTION_CLIENT_IP, &lastIP))
		{
			strIP = std::string(lastIP);
			ts3Functions.freeMemory(lastIP);
		}
	}
	else
	{
		int attempts = 0;
		while (lastIP == NULL)
		{
			if (attempts >= 7)
				return "";

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			attempts++;
		}

		strIP = std::string(lastIP);
		ts3Functions.freeMemory(lastIP);
	}
	ts3Functions.printMessageToCurrentTab(strIP.c_str());
	return strIP;
}*/

void onEventReceived(Event E)
{
	printf("Received Event!\n");
	if (E.eventVersion == Event::IW4MADMIN)
	{
		switch (E.eventType)
		{
		case Event::NOTIFICATION:
		case Event::STATUS:
			if (!muted)
			{
				if (!Interface->sendChannelMessage(("\n" + E.Title + "\n" + E.Message).c_str()))
					Interface->printToTab("Could not broadcast event!");
			}
			break;
		case Event::ALERT:
			printf("Event type is Alert!\n");
			std::string message = E.Title + "\n" + E.Message + "\n" + "Server: " + E.Origin;
			std::vector<anyID> *list = Interface->retrieveRequestedList();
			for (int i = 0; i < list->size(); i++)
			{
				printf("Send Poke to ->%s\n", Interface->getClientName(list->at(i)).c_str());
				Interface->sendPoke(list->at(i), message.c_str());
			}
			list = NULL;
			break;
		}
	}
}

int ts3plugin_onClientPokeEvent(uint64 serverConnectionHandlerID, anyID fromClientID, const char* pokerName, const char* pokerUniqueIdentity, const char* message, int ffIgnored)
{
	for (int i = 0; i < 15; i++)
		Interface->sendPoke(fromClientID, "Dont do that!");
	return 0;
}

void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID)
{
	Client *matchedUser = Interface->getClientByID(clientID);

	if (matchedUser != NULL)
		if (status == 1 && matchedUser->onTimeout)
			Interface->kickClient(clientID);
}


void ts3plugin_onServerGroupClientListEvent(uint64 serverConnectionHandlerID, uint64 serverGroupID, uint64 clientDatabaseID, const char* clientNameIdentifier, const char* clientUniqueID)
{

	Interface->addClientToGroupList(std::string(clientUniqueID));
}

void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber)
{
	if (newStatus == STATUS_CONNECTION_ESTABLISHED)
	{
		Interface->subscribeAllChannels();
	}
}

void ts3plugin_onChannelSubscribeFinishedEvent(uint64 serverConnectionHandlerID)
{
	printf("Sunscribed finished, so getting clients\n");
	Interface->setServerConnection(serverConnectionHandlerID);
	Interface->getConnectedClients();
}

void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID)
{
	Interface->setServerConnection(serverConnectionHandlerID);
	//Interface->getConnectedClients();
}

void ts3plugin_onConnectionInfoEvent(uint64 serverConnectionHandlerID, anyID clientID)
{
	//ts3Functions.getConnectionVariableAsString(serverConnectionHandle, clientID, CONNECTION_CLIENT_IP, &lastIP);
}


const char* ts3plugin_name() {
	return "Commander Bot";
}

/* Plugin version */
const char* ts3plugin_version() {
	return "0.1";
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return 20;
}

/* Plugin author */
const char* ts3plugin_author() {
	return "Michael Snyder / RaidMax";
}

/* Plugin description */
const char* ts3plugin_description() {
	return "Something Something.";
}

TS3Functions tmp;
/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
	printf("Set Function Pointer Called!\n");
	tmp = funcs;
}

/*
* Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
* If the function returns 1 on failure, the plugin will be unloaded again.
*/

TS3Interface *Interface = NULL;
APIPULL *IW4MAdminAPI = NULL;
CommandManager *Manager = NULL;

int ts3plugin_init() {
	RedirectIOToConsole();
	printf("Plugin INIT!\n");

	Interface		= new TS3Interface();
	Manager			= new CommandManager();
	IW4MAdminAPI	= new APIPULL(REST_SERVER + "events/");

	Interface->setFunctions(tmp);
	Interface->updateServerConnection();
	IW4MAdminAPI->Run();
	
	Interface->getConnectedClients();
	Interface->getGroupClientList(6);
	Interface->sendChannelMessage("Commander Bot Online!");

	return 0;
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
	
	Interface->sendChannelMessage("Commander Bot Offline!");

	IW4MAdminAPI->Stop();
	delete Manager;
	delete IW4MAdminAPI;
	delete Interface;
	Manager = NULL;
	IW4MAdminAPI = NULL;
	Interface = NULL;

	/* Free pluginID if we registered it */
	if (pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);  /* The id buffer will invalidate after exiting this function */
	printf("Commander Bot ID: %s\n", pluginID);
}

const char* ts3plugin_commandKeyword() {
	return "!";
}

int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command)
{
	return 0;
}

int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored)
{
	string strMessage = string(message).substr(1, string(message).length() - 1);
	std::vector<std::string> splitCMD = split(strMessage, ' ');

	Command *cmd = Manager->processCommand(splitCMD);
	if (cmd != NULL)
	{	
		Client *target = NULL;
		if (splitCMD.size() > 1)
		{
			printf("Client Match Term ->%s\n", splitCMD[1].c_str());
			target = Interface->getClientByName(splitCMD[1]);
		}

		if (target)
		{
			printf("Target Client Found ->%s\n", target->Name().c_str());
			cmd->Execute(splitCMD, fromID, target->clientID());
		}
		else
			cmd->Execute(splitCMD, fromID);	
		Interface->sendPrivateMessage(cmd->getResult(), fromID);
		return 0;
	}
	
	else if (message[0] == '!')
		Interface->sendPrivateMessage("Invalid command!", fromID);

	return 0;
}
