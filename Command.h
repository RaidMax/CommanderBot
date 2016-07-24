#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
class Command
{
public:
	Command(std::string commandName, std::string(*Execute)(std::vector<std::string> &arguments, unsigned short originID, unsigned short targetID), bool requiresTarget, int numParams, int minLevel);
	bool Execute(unsigned short origin);
	bool Execute(std::vector<std::string> &arguments, unsigned short origin);
	bool Execute(std::vector<std::string> &arguments, unsigned short origin, unsigned short target);
	std::string getName();
	std::string getResult();
private:
	std::string commandName;
	std::string (*executeFnc)(std::vector<std::string> &arguments, unsigned short originID, unsigned short targetID);
	bool requiresTarget;
	int numRequiredParams;
	int minLevel;
	std::string commandResult;
};
#endif
