#include "nodeparser.h"

NodeParser::NodeParser()
{

}

Node* NodeParser::parseFile(const string& rFile) {
	parse(rFile);
	return this->mNodeRoot;
}

bool NodeParser::parseRoot(JSONValue *rJSONValueParent) {
	mNodeRoot=new Node(ContentType::Root);
	mNodeRoot->deserialize(rJSONValueParent);
	return true;
}
