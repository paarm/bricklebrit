#include "nodeparser.h"

NodeParser::NodeParser()
{

}

Node* NodeParser::parseFile(Node *rNodeRoot, const string& rFile) {
	if (rNodeRoot) {
		mNodeRoot=rNodeRoot;
		parse(rFile);
	}
	return this->mNodeRoot;
}

bool NodeParser::parseRoot(JSONValue *rJSONValueParent) {
	mNodeRoot->deserialize(rJSONValueParent);
	return true;
}
