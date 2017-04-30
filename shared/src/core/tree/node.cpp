#include "node.h"
#include <iostream>
#include <fstream>

template <typename T>
T* getXInstanceFromNodeType(bool rCreateNewId) {
	return new T(rCreateNewId);
}

Node * getInstanceFromNodeType(NodeType &rNodeType, bool rCreateNewId) {
	switch (rNodeType) {
		case NodeType::Root:
			return getXInstanceFromNodeType<NodeRoot>(rCreateNewId);
		case NodeType::Project:
			return getXInstanceFromNodeType<NodeProject>(rCreateNewId);
		case NodeType::Node:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
		case NodeType::Sprite:
			return getXInstanceFromNodeType<NodeSprite>(rCreateNewId);
		case NodeType::Scene:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
		case NodeType::SceneRef:
			return getXInstanceFromNodeType<Node>(rCreateNewId);

		case NodeType::TextureAtlas:
			return getXInstanceFromNodeType<NodeTextureAtlas>(rCreateNewId);
		case NodeType::TextureAtlasFrame:
			return getXInstanceFromNodeType<NodeTextureAtlasFrame>(rCreateNewId);

		case NodeType::AnimationSet:
			return getXInstanceFromNodeType<NodeAnimationSet>(rCreateNewId);
		case NodeType::AnimationSetFrameTexture:
			return getXInstanceFromNodeType<NodeAnimationSetFrameTexture>(rCreateNewId);
		case NodeType::AnimationSetFrameTextureAtlas:
			return getXInstanceFromNodeType<NodeAnimationSetFrameTextureAtlas>(rCreateNewId);

		case NodeType::Sound:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
		case NodeType::Music:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
		case NodeType::LastNodeType:
			return nullptr;
	}
	return getXInstanceFromNodeType<Node>(rCreateNewId);
}

static NodeLookupTable gNodeLookupTable[]={
	{NodeType::Node							,"Root"},
	{NodeType::Node							,"Node"},
	{NodeType::Project						,"Project"},
	{NodeType::Scene						,"Scene"},
	{NodeType::SceneRef						,"SceneRef"},
	{NodeType::Sprite						,"Sprite"},
	// Texture Atlas
	{NodeType::TextureAtlas					,"TextureAtlas"},
	{NodeType::TextureAtlasFrame			,"TextureAtlasFrame"},
	// Animation Set
	{NodeType::AnimationSet					,"AnimationSet"},
	{NodeType::AnimationSetFrameTexture		,"AnimationSetFrameTexture"},
	{NodeType::AnimationSetFrameTextureAtlas,"AnimationSetFrameTextureAtlas"},

	{NodeType::Sound						,"Sound"},
	{NodeType::Music						,"Music"},
	{NodeType::LastNodeType					,""},
};

const string getNodeTypeAsString(NodeType &rNodeType) {
	NodeLookupTable *rNodeLookupTable=gNodeLookupTable;
	while(rNodeLookupTable->nodeType!=NodeType::LastNodeType) {
		if (rNodeLookupTable->nodeType==rNodeType) {
			return rNodeLookupTable->nodeName;
		}
		rNodeLookupTable++;
	}
	return "Node";
}

NodeType getNodeTypeFromString(const string &rNodeTypeString) {
	NodeLookupTable *rNodeLookupTable=gNodeLookupTable;
	while(rNodeLookupTable->nodeType!=NodeType::LastNodeType) {
		if (rNodeLookupTable->nodeName==rNodeTypeString) {
			return rNodeLookupTable->nodeType;
		}
		rNodeLookupTable++;
	}
	return NodeType::Node;
}

Node::Node(bool rCreateNewId) : Node(NodeType::Node, rCreateNewId) {
}

Node::Node(NodeType rNodeType) : Node(rNodeType, true) {
}

Node::Node(NodeType rNodeType, bool rCreateNewId) : mNodeType(rNodeType) {
	if (rCreateNewId) {
		setId(NodeIdGenerator::getInstance().getNextNumber());
	}
}

Node::~Node() {
	deleteChildNodes();
}

Node* Node::getParent() {
	return mParent;
}

Node* Node::addChildNode(Node *rNode) {
	mNodes.push_back(rNode);
	Node *rv=mNodes.back();
	rv->mParent=this;
	return rv;
}

void Node::moveChildNode(Node* rNodeToMove) {
	if (rNodeToMove!=this) {
		Node* parent=rNodeToMove->getParent();
		if (parent && parent!=this) {
			bool found=false;
			for (auto it=rNodeToMove->mParent->mNodes.begin();it!=rNodeToMove->mParent->mNodes.end();it++) {
				if (*it.base()==rNodeToMove) {
					found=true;
					rNodeToMove->mParent->mNodes.erase(it);
					break;
				}
			}
			if (found) {
				addChildNode(rNodeToMove);
			}
		}
	}
}

Node* Node::getFirstChildNode() {
	Node *firstChild=nullptr;
	if (!mNodes.empty()) {
		firstChild=mNodes[0];
	}
	return firstChild;
}

void Node::deleteChildNodes() {
	for (auto *rNode : mNodes) {
		delete rNode;
	}
	mNodes.clear();
	mNodes.shrink_to_fit();
	mPropertyMap.clear();
}

void Node::deleteNode(Node *rNodeToDelete) {
	if (rNodeToDelete->mParent) {
		for (auto it=rNodeToDelete->mParent->mNodes.begin();it!=rNodeToDelete->mParent->mNodes.end();it++) {
			if (*it.base()==rNodeToDelete) {
				delete *it.base();
				rNodeToDelete->mParent->mNodes.erase(it);
				break;
			}
		}
	}
}

unsigned long Node::getChildCount() {
	return mNodes.size();
}

NodeType& Node::getNodeType() {
	return mNodeType;
}

void Node::setProperty(const string& rName, PropertyBase* r) {
	if (mPropertyMap.find(rName)!=mPropertyMap.end()) {
		delete mPropertyMap[rName];
	}
	mPropertyMap[rName]=r;
}


void Node::setPropertyString(const string& rName) {
	setProperty(rName, new PropertyString());
}
void Node::setPropertyString(const string& rName, const string &rValue) {
	setProperty(rName, new PropertyString(rValue));
}

void Node::setPropertyFloat(const string& rName) {
	setProperty(rName, new PropertyFloat());
}
void Node::setPropertyFloat(const string& rName, const float &rValue) {
	setProperty(rName, new PropertyFloat(rValue));
}

void Node::setPropertyBool(const string& rName) {
	setProperty(rName, new PropertyBool());
}
void Node::setPropertyBool(const string& rName, const bool &rValue) {
	setProperty(rName, new PropertyBool(rValue));
}

void Node::setPropertyInt(const string& rName) {
	setProperty(rName, new PropertyInt());
}
void Node::setPropertyInt(const string& rName, const int &rValue) {
	setProperty(rName, new PropertyInt(rValue));
}

void Node::setPropertyRectInt(const string& rName) {
	setProperty(rName, new PropertyRectInt());
}
void Node::setPropertyRectInt(const string& rName, const RectInt &rValue) {
	setProperty(rName, new PropertyRectInt(rValue));
}

void Node::setPropertyRectFloat(const string& rName) {
	setProperty(rName, new PropertyRectFloat());
}
void Node::setPropertyRectFloat(const string& rName, const RectFloat &rValue) {
	setProperty(rName, new PropertyRectFloat(rValue));
}

void Node::setPropertyPointInt(const string& rName) {
	setProperty(rName, new PropertyPointInt());
}
void Node::setPropertyPointInt(const string& rName, const PointInt &rValue) {
	setProperty(rName, new PropertyPointInt(rValue));
}

void Node::setPropertyPointFloat(const string& rName) {
	setProperty(rName, new PropertyPointFloat());
}
void Node::setPropertyPointFloat(const string& rName, const PointFloat &rValue) {
	setProperty(rName, new PropertyPointFloat(rValue));
}

void Node::setPropertyList(const string& rName) {
	setProperty(rName, new PropertyList());
}

PropertyBase* Node::getProperty(const string &rName) {
	auto r=mPropertyMap.find(rName);
	if (r!=mPropertyMap.end()) {
		return r->second;
	}
	return nullptr;
}


PropertyString* Node::getPropertyString(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::String) {
		return static_cast<PropertyString*>(p);
	}
	return nullptr;
}

PropertyFloat* Node::getPropertyFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::String) {
		return static_cast<PropertyFloat*>(p);
	}
	return nullptr;
}

PropertyBool* Node::getPropertyBool(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::Bool) {
		return static_cast<PropertyBool*>(p);
	}
	return nullptr;
}

PropertyInt* Node::getPropertyInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::String) {
		return static_cast<PropertyInt*>(p);
	}
	return nullptr;
}

PropertyRectInt* Node::getPropertyRectInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::RectInt) {
		return static_cast<PropertyRectInt*>(p);
	}
	return nullptr;
}

PropertyRectFloat* Node::getPropertyRectFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::RectInt) {
		return static_cast<PropertyRectFloat*>(p);
	}
	return nullptr;
}

PropertyPointInt* Node::getPropertyPointInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::RectInt) {
		return static_cast<PropertyPointInt*>(p);
	}
	return nullptr;
}

PropertyPointFloat* Node::getPropertyPointFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::RectInt) {
		return static_cast<PropertyPointFloat*>(p);
	}
	return nullptr;
}

PropertyList* Node::getPropertyList(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::List) {
		return static_cast<PropertyList*>(p);
	}
	return nullptr;
}


#if 0
template<typename CharT>
class DecimalSeparator : public std::numpunct<CharT>
{
public:
	DecimalSeparator(CharT Separator)
	: m_Separator(Separator)
	{}

protected:
	CharT do_decimal_point()const
	{
		return m_Separator;
	}

private:
	CharT m_Separator;
};
#endif

bool Node::persistSubNode(const string &rFileNameAbs) {
	bool rv=false;
	string rContentString;
	serialize(rContentString,0);
	ofstream outputFile;
	   // tell cout to use our new locale.

	//outputFile.unsetf(ios::showpoint);
	outputFile.open (rFileNameAbs.c_str());
	if (outputFile.is_open()) {
		//outputFile.imbue(std::locale(std::cout.getloc(), new DecimalSeparator<char>('.')));
		outputFile << rContentString;
		outputFile.close();
		rv=true;
	}
	return rv;
}

void Node::serialize(string &buf, unsigned long indent) {
	string rC=string(indent, ' ');
	buf+=rC+"{\n";
	{
		string rT=string(indent+1, ' ');
		buf+=rT+"\"NodeType\": \""+ getNodeTypeAsString(mNodeType)+"\""+",\n";
		buf+=rT+"\"Properties\": [\n";
		if (!mPropertyMap.empty()) {
			string rP=string(indent+2, ' ');
			unsigned long i=0;
			unsigned long count=mPropertyMap.size();
			for(auto &p : mPropertyMap) {
				if (p.second) {
					string rPropertyString=p.second->serializeValue();
					buf+=rP+"{\"name\": \""+p.first+"\", "+rPropertyString+"}";
				}
				if (i+1<count) {
					buf+=",\n";
				} else {
					buf+="\n";
				}
				i++;
			}
		}
		buf+=rT+"]";
		if (!mNodes.empty()) {
			buf+=",\n";
			buf+=rT+"\"ChildNodes\": [\n";
			for (unsigned long i=0, count=mNodes.size();i<count;i++) {
				mNodes.at(i)->serialize(buf, indent+2);
				if (i+1<mNodes.size()) {
					buf+=",\n";
				} else {
					buf+="\n";
				}
			}
			buf+=rT+"]\n";
		} else {
			buf+="\n";
		}
	}
	buf+=rC+"}";
}

void Node::deserialize(JSONValue *rJSONValueParent) {
	string rNodeTypeString=JsonParserBase::extractString(rJSONValueParent, L"NodeType");
	if (rNodeTypeString.empty()) {
		cout << "NodeType not found or empty\n";
	} else {
		NodeType rNodeType=getNodeTypeFromString(rNodeTypeString);
		Node *rChildNode=addChildNode(getInstanceFromNodeType(rNodeType, false));
		rChildNode->deserializeSelf(rJSONValueParent);
	}
}

void Node::deserializeSelf(JSONValue *rJSONValueParent) {
	do {
		if (rJSONValueParent->IsObject()) {
#if 0
			string NodeType=JsonParserBase::extractString(rJSONValueParent, L"NodeType");
			if (NodeType.empty()) {
				cout << "NodeType not found or empty\n";
				break;
			}
#endif

			/* parse all properties */
			JSONValue *rProperties=rJSONValueParent->Child(L"Properties");
			if (rProperties && rProperties->IsArray()) {
				const JSONArray &rJSONArray=rProperties->AsArray();
				for (auto *rJSONValue : rJSONArray) {
					string rName=JsonParserBase::extractString(rJSONValue, L"name");
					if (rName.empty()) {
						cout << "Properties.name not found or empty\n";
						continue;
					}
					string rType=JsonParserBase::extractString(rJSONValue, L"type");
					if (rType.empty()) {
						cout << "Properties.type not found or empty\n";
						continue;
					}
					PropertyType rPropertyType=getPropertyTypeFromName(rType);
					PropertyBase* p=nullptr;
					p=getPropertyInstanceByType(rPropertyType);
					if (p) {
						p->deserializeValue(rJSONValue);
						setProperty(rName, p);
						if (rPropertyType==PropertyType::Int && rName=="Id") {
							PropertyInt *rId=static_cast<PropertyInt*>(p);
							NodeIdGenerator::getInstance().updateLatestNumberIfHigher(rId->value);
						}
					}
				}
			}
			/* parse all child nodes */
			JSONValue *rChildNodes=rJSONValueParent->Child(L"ChildNodes");
			if (rChildNodes && rChildNodes->IsArray()) {
				const JSONArray &rJSONArray=rChildNodes->AsArray();
				for (auto *rJSONValue : rJSONArray) {
					deserialize(rJSONValue);
				}
			}
		}
	} while (0);
}



