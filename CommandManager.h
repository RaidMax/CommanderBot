#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H
#define MAX_COMMANDS 1000
#include "Command.h"
class CommandManager
{
public:
	CommandManager();
	~CommandManager();
	bool addCommand(Command *newCommand);
	Command *processCommand(std::vector<std::string>&);

private:
	Command **commandList;
	int currentCommands;
};
#endif
