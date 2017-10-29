#include "node.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
T* getXInstanceFromNodeType(bool rCreateNewId) {
	return new T(rCreateNewId);
}

Node * getInstanceFromNodeType(NodeType rNodeType, bool rCreateNewId) {
	switch (rNodeType) {
		case NodeType::Root:
			return getXInstanceFromNodeType<NodeRoot>(rCreateNewId);
		case NodeType::Project:
			return getXInstanceFromNodeType<NodeProject>(rCreateNewId);
		case NodeType::Node:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
		case NodeType::Node2d:
			return getXInstanceFromNodeType<Node2d>(rCreateNewId);
		case NodeType::Sprite:
			return getXInstanceFromNodeType<NodeSprite>(rCreateNewId);
		case NodeType::Resource:
			return getXInstanceFromNodeType<NodeResource>(rCreateNewId);
		case NodeType::ResourceInfo:
			return getXInstanceFromNodeType<NodeResourceInfo>(rCreateNewId);
		case NodeType::Texture:
			return getXInstanceFromNodeType<NodeTexture>(rCreateNewId);
		case NodeType::TextureFrame:
			return getXInstanceFromNodeType<NodeTextureFrame>(rCreateNewId);
		case NodeType::Animation:
			return getXInstanceFromNodeType<NodeAnimation>(rCreateNewId);
		case NodeType::AnimationFrame:
			return getXInstanceFromNodeType<NodeAnimationFrame>(rCreateNewId);
		case NodeType::Scene:
			return getXInstanceFromNodeType<NodeScene>(rCreateNewId);
		case NodeType::Layer:
			return getXInstanceFromNodeType<NodeLayer>(rCreateNewId);
		case NodeType::SceneInfo:
			return getXInstanceFromNodeType<NodeSceneInfo>(rCreateNewId);
		case NodeType::SceneRef:
			return getXInstanceFromNodeType<Node>(rCreateNewId);
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
	{NodeType::Root							,"Root"},
	{NodeType::Node							,"Node"},
// drawable
	{NodeType::Node2d						,"Node2d"},
	{NodeType::Sprite						,"Sprite"},
// resources
	{NodeType::Resource						,"Resource"},
	{NodeType::ResourceInfo					,"ResourceInfo"},
	{NodeType::Texture						,"Texture"},
	{NodeType::TextureFrame					,"TextureFrame"},
	{NodeType::Animation					,"Animation"},
	{NodeType::AnimationFrame				,"AnimationFrame"},
	{NodeType::Project						,"Project"},
	{NodeType::Scene						,"Scene"},
	{NodeType::Layer						,"Layer"},
	{NodeType::SceneInfo					,"SceneInfo"},
	{NodeType::SceneRef						,"SceneRef"},

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

Node::Node(bool rCreateNewId) {
	mNodeType=NodeType::Node;
	if (rCreateNewId) {
		setId(NodeIdGenerator::getInstance().allocateNextNumber());
	}
	setName();
}

Node::Node() : Node(true) {
}

//Node::Node(NodeType rNodeType) : Node(rNodeType, true) {
//}

//Node::Node(NodeType rNodeType, bool rCreateNewId) : mNodeType(rNodeType) {
//	if (rCreateNewId) {
//		setId(NodeIdGenerator::getInstance().getNextNumber());
//	}
//}

Node::~Node() {
	deleteChildNodes();

	for(auto &p : mPropertyMap) {
		if (p.second) {
			delete p.second;
		}
	}
	mPropertyMap.clear();
	mPropertyList.clear();

}

Node* Node::getParent() {
	return mParent;
}

vector<Node*> &Node::childs() {
	return mNodes;
}

Node* Node::addChildNode(Node *rNode) {
	mNodes.push_back(rNode);
	Node *rv=mNodes.back();
	rv->mParent=this;
	return rv;
}

Node* Node::getNodeFromIndex(unsigned long rIndex) {
	Node *rv=nullptr;
	if (rIndex<mNodes.size()) {
		rv=mNodes[rIndex];
	}
	return rv;
}

Node* Node::getNodeWithNodeId(int rNodeId) {
	Node *rv=nullptr;
	for (auto *rNode : mNodes) {
		if (rNode->getId()==rNodeId) {
			rv=rNode;
			break;
		}
	}
	return rv;
}

Node* Node::getChildNodeWithNodeIdRecursive(int rNodeId) {
    Node *rv=nullptr;
    rv=getNodeWithNodeId(rNodeId);
    if (!rv) {
        for (auto *rNode : mNodes) {
            rv=rNode->getChildNodeWithNodeIdRecursive(rNodeId);
            if (rv) {
                break;
            }
        }
    }
    return rv;
}

bool Node::isThisNodeOrParentOrGrandParentOf(Node *rNode) {
	bool rv=false;
	if (rNode) {
		if (rNode==this) {
			rv=true;
		} else if (getParent()) {
			rv=getParent()->isThisNodeOrParentOrGrandParentOf(rNode);
		}
	}
	return rv;
}


Node* Node::getChildNodeWithName(const string &rName) {
	Node *rv=nullptr;
	for (auto *rNode : mNodes) {
		if (rNode->getName()==rName) {
			rv=rNode;
			break;
		}
	}
	return rv;
}

Node* Node::getChildNodeWithNameAndNodeType(const string &rName, NodeType rNodeType){
	Node *rv=nullptr;
	for (auto *rNode : mNodes) {
		if (rNode->getNodeType()==rNodeType && rNode->getName()==rName) {
			rv=rNode;
			break;
		}
	}
	return rv;
}

vector<Node*> Node::getChildNodesWithNodeType(NodeType rNodeType) {
	vector<Node*> rv;
	for (auto *rNode : mNodes) {
		if (rNode->getNodeType()==rNodeType) {
			rv.push_back(rNode);
		}
	}
	return rv;
}


bool Node::moveChildNodeUpOrDown(Node *rNode, bool moveUp) {
	bool moved=false;
	if (rNode) {
		size_t cnt=mNodes.size();
		size_t foundAt=0;
		bool found=false;
		for (size_t i=0; i<cnt;i++) {
			if (mNodes.at(i)==rNode) {
				found=true;
				foundAt=i;
			}
		}
		if (!moveUp && found && foundAt+1<cnt) {
			std::swap(mNodes.at(foundAt+1), mNodes.at(foundAt));
			moved=true;
		} else if (moveUp && found && foundAt>0) {
			std::swap(mNodes.at(foundAt-1), mNodes.at(foundAt));
			moved=true;
		}
	}
	return moved;
}

void moveChildNodeUp(Node *rNode);


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
}

void Node::deleteChildNode(Node *rNodeToDelete) {
    for (auto it=mNodes.begin();it!=mNodes.end();it++) {
        if (*it.base()==rNodeToDelete) {
            delete *it.base();
            mNodes.erase(it);
            break;
        }
    }
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
	bool replaced=false;
	r->setPropertyName(rName);
	if (mPropertyMap.find(rName)!=mPropertyMap.end()) {
		PropertyBase* rExist=mPropertyMap[rName];
		if (rExist) {
			for (auto it=mPropertyList.begin(); it!=mPropertyList.end();it++) {
				if (*it.base()==rExist) {
					*it=r;
					replaced=true;
					break;
				}
			}
			delete rExist;
		}
	}
	mPropertyMap[rName]=r;
	if (!replaced) {
		mPropertyList.push_back(r);
	}
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

void Node::setPropertyRef(const string& rName) {
	setProperty(rName, new PropertyRef());
}
void Node::setPropertyRef(const string& rName, const Ref &rValue) {
	setProperty(rName, new PropertyRef(rValue));
}

void Node::setPropertyFrameRef(const string& rName) {
	setProperty(rName, new PropertyFrameRef());
}
void Node::setPropertyFrameRef(const string& rName, const FrameRef &rValue) {
	setProperty(rName, new PropertyFrameRef(rValue));
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

unsigned long Node::getPropertyCount() {
	return mPropertyList.size();
}

PropertyBase* Node::getPropertyFromIndex(unsigned long rIndex) {
	PropertyBase* rv=nullptr;
	if (rIndex<mPropertyList.size()) {
		rv=mPropertyList[rIndex];
	}
	return rv;
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
	if (p && p->getPropertyType()==PropertyType::Float) {
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
	if (p && p->getPropertyType()==PropertyType::Int) {
		return static_cast<PropertyInt*>(p);
	}
	return nullptr;
}

PropertyRef* Node::getPropertyRef(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::Ref) {
		return static_cast<PropertyRef*>(p);
	}
	return nullptr;
}

PropertyFrameRef* Node::getPropertyFrameRef(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::FrameRef) {
		return static_cast<PropertyFrameRef*>(p);
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
	if (p && p->getPropertyType()==PropertyType::RectFloat) {
		return static_cast<PropertyRectFloat*>(p);
	}
	return nullptr;
}

PropertyPointInt* Node::getPropertyPointInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::PointInt) {
		return static_cast<PropertyPointInt*>(p);
	}
	return nullptr;
}

PropertyPointFloat* Node::getPropertyPointFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->getPropertyType()==PropertyType::PointFloat) {
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

bool Node::persistNode(Node *rNode, const string &rFileNameAbs) {
	bool rv=false;
	string rContentString;
	rNode->serialize(rContentString,0);
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

Node* Node::unpersistNode(const string &rFileNameAbs) {
	Node *rNodeRv=nullptr;

	JsonParserBase rJsonParserBase;
	JSONValue*rJSONValueParent=rJsonParserBase.parse(rFileNameAbs);
	if (rJSONValueParent) {
		string rNodeTypeString=JsonParserBase::extractString(rJSONValueParent, L"NodeType");
		if (rNodeTypeString.empty()) {
			cout << "NodeType not found or empty\n";
		} else {
			NodeType rNodeType=getNodeTypeFromString(rNodeTypeString);
			rNodeRv=getInstanceFromNodeType(rNodeType, false);
			rNodeRv->deserializeSelf(rJSONValueParent);
		}
	}
	return rNodeRv;
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
						//if (rPropertyType==PropertyType::Int && rName=="Id") {
						//	PropertyInt *rId=static_cast<PropertyInt*>(p);
						//	NodeIdGenerator::getInstance().updateLatestNumberIfHigher(rId->value);
						//}
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


void Node2d::setResizeHandleSizeLocal(float rResizeHandleSizeLocalX, float rResizeHandleSizeLocalY) {
	glm::mat4 matrix=glm::make_mat4x4(mCurrentModelMatrix.getPointer());

	int w=getSize().x;
	float w2=w/2.0;
	int h=getSize().y;
	float h2=h/2.0;

	for (int cnt=0;cnt<3;cnt++) {
		float startX=w2;
		float startY=h2;
		if (cnt==1) {
			startX=w2;//+rResizeHandleSizeLocalX;
			startY=0-rResizeHandleSizeLocalY/2.0;
		} else if (cnt==2) {
			startX=0-rResizeHandleSizeLocalX/2.0;
			startY=h2;//+rResizeHandleSizeLocalY;
		}

		for (int i=0;i<4;i++) {
			int x=startX;
			int y=startY;
			switch(i) {
			case 0:
				break;
			case 1:
				x+=rResizeHandleSizeLocalX;
				break;
			case 2:
				x+=rResizeHandleSizeLocalX;
				y+=rResizeHandleSizeLocalY;
				break;
			case 3:
				y+=rResizeHandleSizeLocalY;
				break;
			}
			glm::vec4 vh=matrix*glm::vec4{x,y,0.0,1.0};
			if (cnt==0) {
				mResizeHandleBR[i].x=vh.x;
				mResizeHandleBR[i].y=vh.y;
				mResizeHandleLocalBR[i].x=x;
				mResizeHandleLocalBR[i].y=y;
			} else if (cnt==1) {
				mResizeHandleRight[i].x=vh.x;
				mResizeHandleRight[i].y=vh.y;
				mResizeHandleLocalRight[i].x=x;
				mResizeHandleLocalRight[i].y=y;
			} else if (cnt==2) {
				mResizeHandleBottom[i].x=vh.x;
				mResizeHandleBottom[i].y=vh.y;
				mResizeHandleLocalBottom[i].x=x;
				mResizeHandleLocalBottom[i].y=y;
			}
		}
	}
}

void Node2d::setCurrentLocalModelMatrix(GLMMatrix4 &m) {
	mCurrentLocalModelMatrix.setFromPointer(m.getPointer());
	calculateCoords(mCurrentLocalModelMatrix, &mCurrentLocalLocationCenter, &mCurrentLocalLocationBox[0]);
}

GLMMatrix4 Node2d::getCurrentLocalModelMatrix() {
	return mCurrentLocalModelMatrix;
}

void Node2d::setCurrentModelMatrix(GLMMatrix4 &m) {
	mCurrentModelMatrix.setFromPointer(m.getPointer());
	calculateCoords(mCurrentModelMatrix, &mCurrentWorldLocationCenter, &mCurrentWorldLocationBox[0]);
}

GLMMatrix4 Node2d::getCurrentModelMatrix() {
	return mCurrentModelMatrix;
}

void Node2d::calculateCoords(GLMMatrix4 &m, PointFloat* current2LocationCenter, PointFloat* mCurrent4LocationBox) {
	glm::mat4 matrix=glm::make_mat4x4(m.getPointer());

	int w=getSize().x;
	float w2=w/2.0;
	int h=getSize().y;
	float h2=h/2.0;

	glm::vec4 rCenter=matrix*glm::vec4{0.0,0.0,0.0,1.0};
	current2LocationCenter->x=rCenter.x;
	current2LocationCenter->y=rCenter.y;

	// top left
	glm::vec4 rW=matrix*glm::vec4{-w2,-h2,0.0,1.0};
	mCurrent4LocationBox[0].x=rW.x;
	mCurrent4LocationBox[0].y=rW.y;
	// top right
	rW=matrix*glm::vec4{w2,-h2,0.0,1.0};
	mCurrent4LocationBox[1].x=rW.x;
	mCurrent4LocationBox[1].y=rW.y;
	// bottom right
	rW=matrix*glm::vec4{w2,h2,0.0,1.0};
	mCurrent4LocationBox[2].x=rW.x;
	mCurrent4LocationBox[2].y=rW.y;
	// bottom left
	rW=matrix*glm::vec4{-w2,h2,0.0,1.0};
	mCurrent4LocationBox[3].x=rW.x;
	mCurrent4LocationBox[3].y=rW.y;
}


