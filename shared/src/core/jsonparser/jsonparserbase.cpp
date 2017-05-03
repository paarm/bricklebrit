#include "jsonparserbase.h"

JsonParserBase::JsonParserBase() {
}

JsonParserBase::~JsonParserBase() {}

#include <unistd.h>
JSONValue* JsonParserBase::parse(const string &rFileName) {
    cout << "Loading JSON File " << rFileName << endl;

	char cCurrentPath[FILENAME_MAX];
	getcwd(cCurrentPath, sizeof(cCurrentPath));

	cout << "Current working dir: " << cCurrentPath <<endl;
#if 0
	ifstream rFileStreamxx(rFileName);
	string line;
	while ( getline (rFileStreamxx,line) ) {
		cout << line.c_str() << '\n';
	}
	rFileStreamxx.close();
#endif
	ifstream rFileStream(rFileName);
    string rFileString((istreambuf_iterator<char>(rFileStream)), istreambuf_iterator<char>());
	rFileStream.close();

	//cout << rFileString <<endl;

    JSONValue *rJSONValue=JSON::Parse(rFileString.c_str());
    if (rJSONValue && rJSONValue->IsObject()) {
		return rJSONValue;
    }
	return nullptr;
}

const string JsonParserBase::extractString(JSONValue *rJSONValueParent, const wstring &rName) {
    JSONValue *v=rJSONValueParent->Child(rName.c_str());
    if (v && v->IsString()) {
        return string(v->AsStringPtr()->begin(), v->AsStringPtr()->end());
    }
    return "";
}

float JsonParserBase::extractNumber(JSONValue *rJSONValueParent, const wstring &rName) {
    JSONValue *v=rJSONValueParent->Child(rName.c_str());
    if (v && v->IsNumber()) {
        return (float)v->AsNumber();
    }
    return 0.0;
}

bool JsonParserBase::extractNumberExistAsInt(JSONValue *rJSONValueParent, const wstring &rName, int *out) {
    float tmp=0.0;
    (*out)=0;
    if (extractNumberExist(rJSONValueParent, rName, &tmp)) {
        (*out)=(int)tmp;
        return true;
    }
    return false;
}

bool JsonParserBase::extractNumberExist(JSONValue *rJSONValueParent, const wstring &rName, float *out) {
    (*out)=0;
    JSONValue *v=rJSONValueParent->Child(rName.c_str());
    if (v && v->IsNumber()) {
        (*out)=(float)v->AsNumber();
        return true;
    }
    return false;
}

bool JsonParserBase::extractBool(JSONValue *rJSONValueParent, const wstring &rName) {
    JSONValue *v=rJSONValueParent->Child(rName.c_str());
    if (v && v->IsBool()) {
        return v->AsBool();
    }
    return false;
}
