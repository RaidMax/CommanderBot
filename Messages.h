#ifndef MESSAGES_H
#define MESSAGES_H
#include "include/ts3_functions.h"
#include "Client.h"

class TS3Interface
{
public:
	TS3Interface(TS3Functions funcs);
	TS3Interface();
	~TS3Interface();
	void printToTab(std::string message);
	bool sendChannelMessage(std::string message);
	bool sendPoke(anyID clientID, std::string message);
	bool kickClient(anyID clientID);
	bool sendPrivateMessage(std::string message, anyID clientID);
	std::string getClientName(anyID clientID);
	std::string getClientUID(anyID clientID);
	void updateServerConnection();
	void setServerConnection(uint64 server);
	void setFunctions(TS3Functions funcs);
	std::vector<Client> *getConnectedClients();
	bool getGroupClientList(uint64 groupID);
	void addClientToGroupList(std::string);
	void subscribeAllChannels();
	std::vector<anyID> *retrieveRequestedList();
	std::vector<Client> *clientList;
	Client *getClientByID(anyID);
	Client *getClientByName(std::string);
private:
	uint64 serverConnectionHandle;
	TS3Functions interfaceFunctions;
	std::vector<anyID> *groupList;
	std::vector<std::string> groupClientList;
};
#endif
