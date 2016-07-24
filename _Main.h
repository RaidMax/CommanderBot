#ifndef _MAIN_H
#define _MAIN_H
#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

#include "Event.h"
#include "EventPull.h"
#include "Utilities.h"
#include "CommandManager.h"
#include "Messages.h"
#include "include/ts3_functions.h"

static std::string REST_SERVER = "http://server.nbsclan.org:1624/";

extern CommandManager *Manager;
extern TS3Interface *Interface;
extern APIPULL *IW4MAdminAPI;

void onEventReceived(Event E);
#endif
