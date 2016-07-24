#ifndef NETREQUESTS_H
#define NETREQUESTS_H
#include <string>
#include "Poco\JSON\Object.h"

std::string getUri(std::string url);
Poco::JSON::Object::Ptr getJSONObject(std::string text);
int getIntFromJSON(Poco::JSON::Object::Ptr object, std::string key);
std::string getStringFromJSON(Poco::JSON::Object::Ptr object, std::string key);
#endif
