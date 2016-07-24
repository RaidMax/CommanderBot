#include "Client.h"

Client::Client(std::string &name, std::string &uid, anyID cid)
{
	this->clientName = name;
	this->clientUID = uid;
	this->_clientID = cid;
	onTimeout = false;
}

std::string Client::Name()
{
	return this->clientName;
}

std::string Client::UID()
{
	return this->clientUID;
}

anyID Client::clientID()
{
	return this->_clientID;
}
