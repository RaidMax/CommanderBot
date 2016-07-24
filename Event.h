#ifndef EVENT_H
#define EVENT_H
#include <string>

class Event
{
public:
	enum Type
	{
		NOTIFICATION,
		STATUS,
		ALERT
	};

	enum Version
	{
		IW4MADMIN,
	};

	std::string Message;
	std::string Title;

	std::string Origin;
	std::string Target;

	Type eventType;
	Version eventVersion;
};
#endif
