#include "CommandManager.h"
#include "_Main.h"
#include "Commands.h"

#include <vector>

CommandManager::CommandManager()
{
	commandList = new Command*[MAX_COMMANDS];
	currentCommands = 0;

	this->addCommand(new Command("test", testCommand, false, 0, 0));
	this->addCommand(new Command("poke", pokeCommand, false, 0, 0));
	this->addCommand(new Command("status", statusCommand, false, 0, 0));
	this->addCommand(new Command("sudoku", sudokuCommand, false, 0, 0));
	this->addCommand(new Command("server", serverCommand, false, 1, 0));
	this->addCommand(new Command("timeout", timeOutCommand, true, 1, 0));
}

CommandManager::~CommandManager()
{
	for (int i = 0; i < currentCommands; i++)
		delete commandList[i];
	delete commandList;
}

bool CommandManager::addCommand(Command *newCommand)
{
	if (currentCommands == MAX_COMMANDS)
		return false;
	else
	{
		commandList[currentCommands] = newCommand;
		currentCommands++;
		return true;
	}
}

Command *CommandManager::processCommand(vector<std::string> &splitCMD)
{
	if (splitCMD.size() > 0)
	{
		for (int i = 0; i < currentCommands; i++)
			if (commandList[i]->getName() == splitCMD[0])
				return commandList[i];
	}

	return NULL;
}
