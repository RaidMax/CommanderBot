#include "Utilities.h"
#include <sstream>
#include <algorithm>

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		if (!item.empty())
			elems.push_back(item);
	}
	return elems;
}


vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void FindAndReplace(std::string& source, const char* find, const char* replace)
{
	//ASSERT(find != NULL);
	//ASSERT(replace != NULL);
	size_t findLen = strlen(find);
	size_t replaceLen = strlen(replace);
	size_t pos = 0;

	//search for the next occurrence of find within source
	while ((pos = source.find(find, pos)) != std::string::npos)
	{
		//replace the found string with the replacement
		source.replace(pos, findLen, replace);

		//the next line keeps you from searching your replace string, 
		//so your could replace "hello" with "hello world" 
		//and not have it blow chunks.
		pos += replaceLen;
	}
}
