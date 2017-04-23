#pragma once

#include <vector>
#include <map>
#include <string>
#include "property/property.h"
#include "../jsonparser/jsonparser.h"

enum class ContentType {
	Root,
	Project,
	Node,
	World,
	Scene,
	SceneRef,
	Sprite,
	AnimatedSprite,
	SpriteFrameGroup,
	SpriteFrame,
	Texture,
	Sound,
	Music
};

using namespace std;
class Node
{
private:
	vector<Node*>				mNodes;
	Node*						mParent=nullptr;
	map<string, PropertyBase*>	mPropertyMap;
	ContentType					mContentType=ContentType::Node;
	template<typename T>
	void setProperty(const string& rName, const T &rValue, PropertyType rPropertyType, PropertyBaseValue<T>*r);
	template<typename T>
	void setProperty(const string& rName, PropertyType rPropertyType, PropertyBaseValue<T>*r);
public:
	Node();
	Node(ContentType rContentType);
	virtual ~Node();
	Node* addChildNode(Node*);
	void deleteChildNodes();
	void deleteNode(Node *rNodeToDelete);
	unsigned long getChildCount();

	void setPropertyString(const string& rName, const string &rValue);
	void setPropertyFloat(const string& rName, const float &rValue);
	void setPropertyInt(const string& rName, const int &rValue);
	void setPropertyRectInt(const string& rName, const RectInt &rValue);
	void setPropertyRectFloat(const string& rName, const RectFloat &rValue);
	void setPropertyPointInt(const string& rName, const PointInt &rValue);
	void setPropertyPointFloat(const string& rName, const PointFloat &rValue);

	PropertyBase* getProperty(const string &rName);
	PropertyString* getPropertyString(const string &rName);
	PropertyFloat* getPropertyFloat(const string &rName);
	PropertyInt* getPropertyInt(const string &rName);
	PropertyRectInt* getPropertyRectInt(const string &rName);
	PropertyRectFloat* getPropertyRectFloat(const string &rName);
	PropertyPointInt* getPropertyPointInt(const string &rName);
	PropertyPointFloat* getPropertyPointFloat(const string &rName);

	void serialize(string &buf, unsigned long indent);
	void deserialize(JSONValue *rJSONValueParent);
};

const string getContentTypeAsString(ContentType &rContentType);
ContentType getContentTypeFromString(const string &rContentTypeString);
Node * getInstanceFromContentType(ContentType &rContentType);

#define PROPERTY_STRING(fieldName) \
void set##fieldName(const string& r##fieldName) { \
	setPropertyString (#fieldName, r##fieldName); \
} \
const string& get##fieldName() { \
	return getPropertyString (#fieldName)->value; \
}

#define PROPERTY_FLOAT(fieldName) \
void set##fieldName(const float& r##fieldName) { \
	setPropertyFloat (#fieldName, r##fieldName); \
} \
const float& get##fieldName() { \
	return getPropertyFloat (#fieldName)->value; \
}

#define PROPERTY_INT(fieldName) \
void set##fieldName(const int& r##fieldName) { \
	setPropertyInt (#fieldName, r##fieldName); \
} \
const int& get##fieldName() { \
	return getPropertyInt (#fieldName)->value; \
}

#define PROPERTY_RECTINT(fieldName) \
void set##fieldName(const RectInt& r##fieldName) { \
	setPropertyRectInt (#fieldName, r##fieldName); \
} \
const RectInt& get##fieldName() { \
	return getPropertyRectInt (#fieldName)->value; \
}

#define PROPERTY_RECTFLOAT(fieldName) \
void set##fieldName(const RectFloat& r##fieldName) { \
	setPropertyRectFloat (#fieldName, r##fieldName); \
} \
const RectFloat& get##fieldName() { \
	return getPropertyRectFloat (#fieldName)->value; \
}

#define PROPERTY_POINTINT(fieldName) \
void set##fieldName(const PointInt& r##fieldName) { \
	setPropertyPointInt (#fieldName, r##fieldName); \
} \
const PointInt& get##fieldName() { \
	return getPropertyPointInt (#fieldName)->value; \
}

#define PROPERTY_POINTFLOAT(fieldName) \
void set##fieldName(const PointFloat& r##fieldName) { \
	setPropertyPointFloat (#fieldName, r##fieldName); \
} \
const PointFloat& get##fieldName() { \
	return getPropertyPointFloat (#fieldName)->value; \
}


class NodeRoot : public Node {
private:
public:
	NodeRoot() : Node(ContentType::Root) {
	}
};

class NodeProject : public Node {
private:
public:
	PROPERTY_STRING(ProjectName)
	PROPERTY_STRING(StartScene)
	NodeProject() : Node(ContentType::Project) {
		setProjectName("");
		setStartScene("");
	}
};

class NodeSprite : public Node {
private:
public:
	PROPERTY_STRING(Name)
	PROPERTY_STRING(Texture)
	PROPERTY_POINTINT(Position)
	PROPERTY_RECTINT(Size)
	PROPERTY_RECTINT(TextureSourceRect)
	PROPERTY_POINTFLOAT(Scale)
	PROPERTY_FLOAT(Rotation)


	NodeSprite() : Node(ContentType::Sprite) {
		setName("");
		setTexture("res://");
		setPosition(PointInt(0,0));
		setSize(RectInt(0,0,0,0));
		setTextureSourceRect(RectInt(0,0,0,0));
		setScale(PointFloat(1.0,1.0));
		setRotation(0);
	}
};


