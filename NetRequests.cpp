#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/StreamCopier.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/URI.h"

using namespace Poco;
using namespace std;
using namespace Poco::Net;

std::string getUri(std::string url)
{
	URI reqURI(url);
	Net::HTTPClientSession session(reqURI.getHost(), reqURI.getPort());

	try
	{
		string getPath(reqURI.getPathAndQuery());

		if (getPath.empty())
			getPath = "/";

		Net::HTTPRequest pocoREQ(Net::HTTPRequest::HTTP_GET, getPath, Net::HTTPMessage::HTTP_1_1);
		session.sendRequest(pocoREQ);

		HTTPResponse res;
		//std::cout << res.getStatus() << " " << res.getReason() << endl;

		if (res.getStatus() != HTTPResponse::HTTPStatus::HTTP_OK)
			return "";
		istream &is = session.receiveResponse(res);
#if _MSC_VER == 1800 
		return std::string(istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
#elif _MSC_VER > 1800
		return string(istreambuf_iterator<char>(is), {});
#endif
	}


	catch (Exception)
	{
		return "";
	}
}

JSON::Object::Ptr getJSONObject(string text)
{
	JSON::Parser parser;
	Dynamic::Var result = parser.parse(text);
	JSON::Object::Ptr arr = result.extract<JSON::Object::Ptr>();
	return arr;
}

int getIntFromJSON(JSON::Object::Ptr object, string key)
{
	return object->getValue<int>(key);
}

string getStringFromJSON(JSON::Object::Ptr object, string key)
{
	return object->getValue<string>(key);
}
