#include "Commands.h"
#include "Messages.h"
#include "NetRequests.h"
#include "_Main.h"
#include "Poco\URI.h"

string testCommand(commandArgs)
{
	return "Test command success!";
}

string pokeCommand(commandArgs)
{
	Interface->sendPoke(originID, "Hey, Wakeup!");
	return "VOID";
}

string statusCommand(commandArgs)
{
	IW4MAdminAPI->addToStatusQueue("events?status=1/");
	return "Requested Status";
}

string sudokuCommand(commandArgs)
{
	Interface->sendChannelMessage(Interface->getClientName(originID) + " has committed suicide!");
	Interface->kickClient(originID);

	return "VOID";
}

string serverCommand(commandArgs)
{
	// this command should be gaurenteed to have at least 1 argument
	std::string encoded;

	for (int i = 1; i < arguments.size(); i++)
	{
		std::string arg = arguments[i];
		std::string enc;
		if (i == 1)
			arg = "!" + arg;

		Poco::URI::encode(arg + " ", "", enc);
		encoded += enc;
	}

	std::string id = Interface->getClientUID(originID);
	//fix poco issue
	FindAndReplace(id, "+", "%2b");
	FindAndReplace(encoded, "+", "%2b");
	
#ifdef _DEBUG
	printf("Encoded data is ->%s\n", encoded.c_str());
	printf("Origin clientID ->%s\n", id.c_str());
	printf("URI request is ->%s\n", (REST_SERVER + "command?query=" + encoded + "&uid=" + id).c_str());
#endif

	std::string response;

	if (arguments[1] == "select" && arguments.size() > 2)
		response = getUri(REST_SERVER + "command?select=" + arguments[2] + "&uid=" + id);
	else
		response = getUri(REST_SERVER + "command?query=" + encoded + "&uid=" + id);
	
	if (response.length() > 0)
		FindAndReplace(response, "<br/>", "\n");
	
	return response;
}

string timeOutCommand(commandArgs)
{
	if (!Interface->getClientByID(targetID)->onTimeout)
	{
		Interface->getClientByID(targetID)->onTimeout = true;
		return "User has been placed in timeout.";
	}

	else
	{
		Interface->getClientByID(targetID)->onTimeout = false;
		return "User is no longer on timeout.";
	}
}