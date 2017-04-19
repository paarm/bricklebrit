#pragma once
#include "../jsonparser/jsonparser.h"
#include "node.h"

class NodeParser : public JsonParserBase
{
private:
	Node *		mNodeRoot=nullptr;
protected:
	virtual bool parseRoot(JSONValue *rJSONValueParent);
public:
	NodeParser();
	Node* parseFile(const string& rFile);
};

