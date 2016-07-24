#include "teamspeak\public_definitions.h"
#include <string>

class Client
{
public:
	Client(std::string &name, std::string &uid, anyID cid);
	std::string Name();
	std::string UID();
	anyID clientID();
	bool onTimeout;
private:
	std::string clientName;
	std::string clientUID;
	anyID _clientID;
};