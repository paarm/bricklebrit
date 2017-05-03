#ifndef JSONPARSERBASE_H
#define JSONPARSERBASE_H

#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>
#include "../thirdparty/simplejson/JSONValue.h"

using namespace std;

class JsonParserBase {
public:
	static const string extractString(JSONValue *rJSONValueParent, const wstring &rName);
	static float extractNumber(JSONValue *rJSONValueParent, const wstring &rName);
	static bool extractNumberExist(JSONValue *rJSONValueParent, const wstring &rName, float *out);
	static bool extractNumberExistAsInt(JSONValue *rJSONValueParent, const wstring &rName, int *out);
	static bool extractBool(JSONValue *rJSONValueParent, const wstring &rName);
	JsonParserBase();
	virtual ~JsonParserBase();
	JSONValue* parse(const string &rFileName);
};

#endif // JSONPARSERBASE_H
