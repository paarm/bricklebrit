#include "node.h"
#include <iostream>
#include <fstream>


const string getContentTypeAsString(ContentType &rContentType) {
	switch (rContentType) {
		case ContentType::Root:
		return "Root";
		case ContentType::Project:
			return "Project";
		case ContentType::Node:
			return "Node";
		case ContentType::World:
			return "World";
		case ContentType::Scene:
			return "Scene";
		case ContentType::SceneRef:
			return "SceneRef";
		case ContentType::Sprite:
			return "Sprite";
		case ContentType::AnimatedSprite:
			return "AnimatedSprite";
		case ContentType::SpriteFrameGroup:
			return "SpriteFrameGroup";
		case ContentType::SpriteFrame:
			return "SpriteFrame";
		case ContentType::Texture:
			return "Texture";
		case ContentType::Sound:
			return "Sound";
		case ContentType::Music:
			return "Music";
	}
	return "Node";
}

ContentType getContentTypeFromString(const string &rContentTypeString) {
	if (rContentTypeString=="Root") {
		return ContentType::Root;
	} else if (rContentTypeString=="Node") {
		return ContentType::Node;
	} else if (rContentTypeString=="Project") {
		return ContentType::Project;
	} else if (rContentTypeString=="World") {
		return ContentType::World;
	} else if (rContentTypeString=="Scene") {
		return ContentType::Scene;
	} else if (rContentTypeString=="SceneRef") {
		return ContentType::SceneRef;
	} else if (rContentTypeString=="Sprite") {
		return ContentType::Sprite;
	} else if (rContentTypeString=="AnimatedSprite") {
		return ContentType::AnimatedSprite;
	} else if (rContentTypeString=="SpriteFrameGroup") {
		return ContentType::SpriteFrameGroup;
	} else if (rContentTypeString=="SpriteFrame") {
		return ContentType::SpriteFrame;
	} else if (rContentTypeString=="Texture") {
		return ContentType::Texture;
	} else if (rContentTypeString=="Sound") {
		return ContentType::Sound;
	} else if (rContentTypeString=="Music") {
		return ContentType::Music;
	}
	return ContentType::Node;
}

Node * getInstanceFromContentType(ContentType &rContentType) {
	switch (rContentType) {
		case ContentType::Root:
			return new NodeRoot();
		case ContentType::Project:
			return new NodeProject();
		case ContentType::Node:
			return new Node();
		case ContentType::Sprite:
			return new NodeSprite();
		case ContentType::World:
			return new Node();
		case ContentType::Scene:
			return new Node();
		case ContentType::SceneRef:
			return new Node();
		case ContentType::AnimatedSprite:
			return new Node();
		case ContentType::SpriteFrameGroup:
			return new Node();
		case ContentType::SpriteFrame:
			return new Node();
		case ContentType::Texture:
			return new Node();
		case ContentType::Sound:
			return new Node();
		case ContentType::Music:
			return new Node();
	}
	return new Node();
}

Node::Node() : Node(ContentType::Node) {
}

Node::Node(ContentType rContentType) : mContentType(rContentType) {

#if 0
	if (mContentType==ContentType::World) {
		setPropertyString("Name", "World");
	} else if (mContentType==ContentType::Sprite) {
		setPropertyString	("Name", "Sprite Name");
		setPropertyPointInt	("Position", PointInt(0,0));
		setPropertyRectInt	("Size", RectInt(0,0,0,0));
		setPropertyPointFloat("Scale", PointFloat(1.0,1.0));
		setPropertyFloat	("Rotation", 0.0);
		setPropertyString	("Texture", "res://");
		setPropertyRectInt	("TextureSourceRect", RectInt(0,0,0,0));
	} else if (mContentType==ContentType::Project) {
	}
#endif
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

ContentType& Node::getContentType() {
	return mContentType;
}

void Node::setProperty(const string& rName, PropertyBase* r) {
	if (mPropertyMap.find(rName)!=mPropertyMap.end()) {
		delete mPropertyMap[rName];
	}
	mPropertyMap[rName]=r;
}

void Node::setPropertyString(const string& rName, const string &rValue) {
	setProperty(rName, new PropertyString(rValue));
}
void Node::setPropertyFloat(const string& rName, const float &rValue) {
	setProperty(rName, new PropertyFloat(rValue));
}
void Node::setPropertyInt(const string& rName, const int &rValue) {
	setProperty(rName, new PropertyInt(rValue));
}
void Node::setPropertyRectInt(const string& rName, const RectInt &rValue) {
	setProperty(rName, new PropertyRectInt(rValue));
}
void Node::setPropertyRectFloat(const string& rName, const RectFloat &rValue) {
	setProperty(rName, new PropertyRectFloat(rValue));
}
void Node::setPropertyPointInt(const string& rName, const PointInt &rValue) {
	setProperty(rName, new PropertyPointInt(rValue));
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
	if (p && p->propertyType==PropertyType::String) {
		return static_cast<PropertyString*>(p);
	}
	return nullptr;
}

PropertyFloat* Node::getPropertyFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::String) {
		return static_cast<PropertyFloat*>(p);
	}
	return nullptr;
}

PropertyInt* Node::getPropertyInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::String) {
		return static_cast<PropertyInt*>(p);
	}
	return nullptr;
}

PropertyRectInt* Node::getPropertyRectInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::RectInt) {
		return static_cast<PropertyRectInt*>(p);
	}
	return nullptr;
}

PropertyRectFloat* Node::getPropertyRectFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::RectInt) {
		return static_cast<PropertyRectFloat*>(p);
	}
	return nullptr;
}

PropertyPointInt* Node::getPropertyPointInt(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::RectInt) {
		return static_cast<PropertyPointInt*>(p);
	}
	return nullptr;
}

PropertyPointFloat* Node::getPropertyPointFloat(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::RectInt) {
		return static_cast<PropertyPointFloat*>(p);
	}
	return nullptr;
}

PropertyList* Node::getPropertyList(const string &rName) {
	PropertyBase *p=getProperty(rName);
	if (p && p->propertyType==PropertyType::List) {
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
		buf+=rT+"\"ContentType\": \""+ getContentTypeAsString(mContentType)+"\""+",\n";
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
	string contentType=JsonParserBase::extractString(rJSONValueParent, L"ContentType");
	if (contentType.empty()) {
		cout << "ContentType not found or empty\n";
	} else {
		ContentType rContentType=getContentTypeFromString(contentType);
		Node *rChildNode=addChildNode(getInstanceFromContentType(rContentType));
		rChildNode->deserializeSelf(rJSONValueParent);
	}
}

void Node::deserializeSelf(JSONValue *rJSONValueParent) {
	do {
		if (rJSONValueParent->IsObject()) {
#if 0
			string contentType=JsonParserBase::extractString(rJSONValueParent, L"ContentType");
			if (contentType.empty()) {
				cout << "ContentType not found or empty\n";
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



