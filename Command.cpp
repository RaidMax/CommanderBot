#include "Command.h"

Command::Command(std::string commandName, std::string(*Execute)(std::vector<std::string> &arguments, unsigned short originID, unsigned short targetID), bool requiresTarget, int numParams, int minLevel)
{
	// lets assume good data;
	this->commandName = commandName;
	this->executeFnc = Execute;
	this->requiresTarget = requiresTarget;
	this->numRequiredParams = numParams;
	this->minLevel = minLevel;
}

bool Command::Execute(unsigned short origin)
{
	std::vector<std::string> satisfy;
	return this->Execute(satisfy, origin, 0);
}

bool Command::Execute(std::vector<std::string> &arguments, unsigned short origin)
{
	return this->Execute(arguments, origin, 0);
}

bool Command::Execute(std::vector<std::string> &arguments, unsigned short origin, unsigned short target)
{
	if (requiresTarget && !target)
	{
		commandResult = "This commmand requires a target client.";
		return false;
	}

	else if (arguments.size() < numRequiredParams)
	{
		commandResult = "This commmand requires at least " + std::to_string(numRequiredParams) + " parameters";
		return false;
	}
	
	try
	{
		commandResult = this->executeFnc(arguments, origin, target);
	}

	catch (std::exception &e)
	{
		printf("Command generated an exception-> %s\n", e.what());
		return false;
	}

	return true;
}

std::string Command::getName()
{
	return commandName;
}

std::string Command::getResult()
{
	return commandResult;
}
