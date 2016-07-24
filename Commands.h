#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
using namespace std;
#define commandArgs vector<string> &arguments, unsigned short originID, unsigned short targetID

string testCommand(commandArgs);
string pokeCommand(commandArgs);
string statusCommand(commandArgs);
string serverCommand(commandArgs);
string sudokuCommand(commandArgs);
string timeOutCommand(commandArgs);
#endif
