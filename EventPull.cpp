#define MAX_QUEUE 10
#include "EventPull.h"
#include "_Main.h"
#include "Poco\JSON\JSON.h";
#include "Poco\JSON\Object.h"
#include "NetRequests.h"

APIPULL::APIPULL(std::string URI) : URI(URI) 
{
	statusQueue = new queue<string>();
}

APIPULL::~APIPULL()
{
	delete statusQueue;
	statusQueue = NULL;
}

void APIPULL::Run()
{
	isRunning = true;
	apiThread = std::thread(&APIPULL::doWork, this);
	apiThread.detach();
}

void APIPULL::doWork()
{
	std::string restJSON; 
	Poco::JSON::Object::Ptr jsonArr;

	while (isRunning && statusQueue != NULL)
	{
		std::this_thread::sleep_for(std::chrono::seconds((int)isRunning));

		try 
		{
			//printf("Requesting info from REST server... %i\n", (int)isRunning);

			if (statusQueue == NULL)
				return;

			if (!statusQueue->empty())
			{
				string test = statusQueue->front();
				restJSON = getUri(test);
				statusQueue->pop();
			}
			else
				restJSON = getUri(URI);
			jsonArr = getJSONObject(restJSON);
		}

		catch (Poco::Exception)
		{
			//printf("Caught POCO Exception while getting API JSON\n");
			continue;
		}

		catch (std::exception)
		{
			printf("API failed to get JSON\n");
		}

		if (jsonArr->size() == 0)
			continue;

		Event e;

		e.Message = jsonArr->get("Message").convert<std::string>();
		e.Title = jsonArr->get("Title").convert<std::string>();
		e.Origin = jsonArr->get("Origin").convert<std::string>();
		e.Target = jsonArr->get("Target").convert<std::string>();
		e.eventType = (Event::Type)jsonArr->get("Type").convert<int>();
		e.eventVersion = (Event::Version)jsonArr->get("Version").convert<int>();

		onEventReceived(e);
	}
}

void APIPULL::addToStatusQueue(const std::string &URI)
{
	if (statusQueue && statusQueue->size() < MAX_QUEUE)
		statusQueue->push(REST_SERVER + URI);
	
	printf("Last request is ->%s\n", (REST_SERVER + URI).c_str());
}

void APIPULL::Stop()
{
	isRunning = false;
	delete statusQueue;
	statusQueue = NULL;
}
