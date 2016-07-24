#ifndef EVENTPULL_H
#define EVENTPULL_H
#include <thread>
#include <string>
#include <queue>

class APIPULL
{
	std::thread apiThread;
	std::string URI;
	bool isRunning;
	std::queue<std::string> *statusQueue;
	void doWork();

public:
	APIPULL(std::string URI);
	~APIPULL();
	void Run();
	void addToStatusQueue(const std::string &URI);
	void Stop();
};
#endif
