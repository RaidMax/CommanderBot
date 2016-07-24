#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <vector>

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
void FindAndReplace(std::string& source, const char* find, const char* replace);
#endif

