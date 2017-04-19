#pragma once

#include <vector>
#include <map>
#include <string>
#include "property/property.h"
#include "../jsonparser/jsonparser.h"

enum class ContentType {
	Root,
	Container,
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

const string getContentTypeAsString(ContentType &rContentType);
ContentType getContentTypeFromString(const string &rContentTypeString);

using namespace std;
class Node
{
private:
	vector<Node>				mNodes;
	Node*						mParent=nullptr;
	map<string, PropertyBase*>	mPropertyMap;
	ContentType					mContentType=ContentType::Container;
	template<typename T>
	void setProperty(const string& rName, const T &rValue, PropertyType rPropertyType, PropertyBaseValue<T>*r);
	template<typename T>
	void setProperty(const string& rName, PropertyType rPropertyType, PropertyBaseValue<T>*r);
public:
	Node(ContentType rContentType);
	~Node();
	Node& addChildNode(ContentType rContentType);
	void deleteNode(Node &rNodeToDelete);
	unsigned long getChildCount();

	void setPropertyString(const string& rName, const string &rValue);
	void setPropertyFloat(const string& rName, const float &rValue);
	void setPropertyInt(const string& rName, const int &rValue);
	void setPropertyRectInt(const string& rName, const RectInt &rValue);
	void setPropertyRectFloat(const string& rName, const RectFloat &rValue);
	void setPropertyPointInt(const string& rName, const PointInt &rValue);
	void setPropertyPointFloat(const string& rName, const PointFloat &rValue);

	PropertyBase* getProperty(const string &rName);
	PropertyRectInt* getPropertyRectInt(const string &rName);
	void serialize(string &buf, unsigned long indent);
	void deserialize(JSONValue *rJSONValueParent);
};
