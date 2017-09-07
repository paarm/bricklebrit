#pragma once

#include <vector>
#include <map>
#include <string>
#include "property/property.h"
#include "../jsonparser/jsonparser.h"
#include "nodeidgenerator.h"
#include <future>


enum class NodeType {
	Root,
	Node,
// Drawable
	Node2d,
	Sprite,
// Resource
	Resource,
	Texture,
	TextureFrame,
	Project,
	Scene,
	SceneRef,
	TextureAtlas,
	TextureAtlasFrame,
	AnimationSet,
	AnimationSetFrameTexture,
	AnimationSetFrameTextureAtlas,


	Sound,
	Music,
	LastNodeType
};

class Node;

struct NodeLookupTable {
	NodeType	nodeType;
	string		nodeName;
	//std::function<Node*(NodeType rNodeType, bool rCreateNewId)> createNodeFct;
};

using namespace std;
class Node
{
private:
	vector<Node*>				mNodes;
	Node*						mParent=nullptr;
	map<string, PropertyBase*>	mPropertyMap;
	vector<PropertyBase*>		mPropertyList;
	void setProperty(const string& rName, PropertyBase* r);
	void deserializeSelf(JSONValue *rJSONValueParent);
protected:
	NodeType					mNodeType=NodeType::Node;
public:
	Node(bool rCreateNewId);
	Node();
	//Node(NodeType rNodeType);
	//Node(NodeType rNodeType, bool rCreateNewId);
	~Node();
	Node* addChildNode(Node*);
	Node* getParent();
	Node* getFirstChildNode();
	void moveChildNode(Node* rNodeToMove);
	void deleteChildNodes();
	void deleteNode(Node *rNodeToDelete);
	unsigned long getChildCount();
	Node* getNodeFromIndex(unsigned long rIndex);
	Node* getNodeWithNodeId(int rIndex);
	Node* getChildNodeWithName(const string &rName);

	NodeType& getNodeType();
	void setPropertyString(const string& rName);
	void setPropertyString(const string& rName, const string &rValue);
	void setPropertyFloat(const string& rName);
	void setPropertyFloat(const string& rName, const float &rValue);
	void setPropertyBool(const string& rName);
	void setPropertyBool(const string& rName, const bool &rValue);
	void setPropertyInt(const string& rName);
	void setPropertyInt(const string& rName, const int &rValue);
	void setPropertyRef(const string& rName);
	void setPropertyRef(const string& rName, const Ref &rValue);
	void setPropertyFrameRef(const string& rName);
	void setPropertyFrameRef(const string& rName, const FrameRef &rValue);
	void setPropertyRectInt(const string& rName);
	void setPropertyRectInt(const string& rName, const RectInt &rValue);
	void setPropertyRectFloat(const string& rName);
	void setPropertyRectFloat(const string& rName, const RectFloat &rValue);
	void setPropertyPointInt(const string& rName);
	void setPropertyPointInt(const string& rName, const PointInt &rValue);
	void setPropertyPointFloat(const string& rName);
	void setPropertyPointFloat(const string& rName, const PointFloat &rValue);
	void setPropertyList(const string& rName);

	PropertyBase* getProperty(const string &rName);
	PropertyString* getPropertyString(const string &rName);
	PropertyFloat* getPropertyFloat(const string &rName);
	PropertyBool* getPropertyBool(const string &rName);
	PropertyInt* getPropertyInt(const string &rName);
	PropertyRef* getPropertyRef(const string &rName);
	PropertyFrameRef* getPropertyFrameRef(const string &rName);
	PropertyRectInt* getPropertyRectInt(const string &rName);
	PropertyRectFloat* getPropertyRectFloat(const string &rName);
	PropertyPointInt* getPropertyPointInt(const string &rName);
	PropertyPointFloat* getPropertyPointFloat(const string &rName);
	PropertyList* getPropertyList(const string &rName);

	unsigned long getPropertyCount();
	PropertyBase* getPropertyFromIndex(unsigned long rIndex);

	static bool persistNode(Node *rNode, const string &rFileNameAbs);
	static Node* unpersistNode(const string &rFileNameAbs);
	void serialize(string &buf, unsigned long indent);
	void deserialize(JSONValue *rJSONValueParent);

	PROPERTY_STRING_GETSET(Name)
	PROPERTY_INT_GETSET(Id)
};

const string getNodeTypeAsString(NodeType &rNodeType);
NodeType getNodeTypeFromString(const string &rNodeTypeString);
//Node * getInstanceFromNodeType(NodeType &rNodeType);
template <typename T>
T* getXInstanceFromNodeType(bool rCreateNewId);
Node * getInstanceFromNodeType(NodeType &rNodeType, bool rCreateNewId);


class NodeRoot : public Node {
private:
public:
	NodeRoot(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Root;
	}
	NodeRoot() : NodeRoot(true) {
	}
};

class NodeProject : public Node {
private:
public:
	PROPERTY_STRING_GETSET(ScenesSubPath)
	PROPERTY_STRING_GETSET(ResourcesSubPath)
	PROPERTY_STRING_GETSET(StartScene)
	PROPERTY_STRING_GETSET(ProjectResource)
	PROPERTY_INT_GETSET(NextFreeId)
	PROPERTY_LIST_GETSET(ListTest)

	NodeProject(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Project;
		setScenesSubPath("");
		setResourcesSubPath("");
		setStartScene("");
		setProjectResource("");
		setNextFreeId(0);
		setListTest();
	}

	NodeProject() : NodeProject(true) {
	}
};

class Node2d : public Node {
private:
public:
	PROPERTY_POINTINT_GETSET(Position)
	PROPERTY_POINTINT_GETSET(Size)
	PROPERTY_POINTFLOAT_GETSET(Scale)
	PROPERTY_FLOAT_GETSET(Rotation)
	PROPERTY_BOOL_GETSET(EditorSelected)

	Node2d(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Node2d;
		setPosition();
		setSize();
		setScale(PointFloat(1.0,1.0));
		setRotation();
		setEditorSelected(false);
	}

	Node2d() : Node2d(true) {
	}

};

class NodeScene : public Node2d {
private:
public:

	NodeScene(bool rCreateNewId) : Node2d(rCreateNewId) {
		mNodeType=NodeType::Scene;
	}

	NodeScene() : NodeScene(true) {
	}
};

class NodeSprite : public Node2d {
private:
public:
	PROPERTY_BOOL_GETSET(IsAnimated)
	// if isAnimated==true
		PROPERTY_FRAMEREF_GETSET(FrameRef)
	// else
		PROPERTY_STRING_GETSET(DefaultAnimation)
	// end if


	NodeSprite(bool rCreateNewId) : Node2d(rCreateNewId) {
		mNodeType=NodeType::Sprite;
		setIsAnimated(false);
		setFrameRef();
		setDefaultAnimation();
	}

	NodeSprite() : NodeSprite(true) {
	}
};



class NodeResource : public Node {
private:
public:
	NodeResource(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Resource;
	}

	NodeResource() : NodeResource(true) {
	}
};

class NodeTexture : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Path)
	NodeTexture(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Texture;
		setPath("");
	}

	NodeTexture() : NodeTexture(true) {
	}
};

class NodeTextureFrame : public Node {
private:
public:
	PROPERTY_RECTINT_GETSET(Frame)
	NodeTextureFrame(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::TextureFrame;
		setFrame();
	}

	NodeTextureFrame() : NodeTextureFrame(true) {
	}
};


class NodeTextureAtlas : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)
	PROPERTY_STRING_GETSET(Texture)

	NodeTextureAtlas(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::TextureAtlas;
		setName();
		setTexture();
	}

	NodeTextureAtlas() : NodeTextureAtlas(true) {
	}
};

class NodeTextureAtlasFrame : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)
	PROPERTY_RECTINT_GETSET(TextureSourceRect)
	PROPERTY_INT_GETSET(Rotation)

	NodeTextureAtlasFrame(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::TextureAtlasFrame;
		setName();
		setTextureSourceRect();
		setRotation(0); // 0, 90=rotated Right, 270=rotated Left
	}

	NodeTextureAtlasFrame() : NodeTextureAtlasFrame(true) {
	}
};

class NodeAnimationSet : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)

	NodeAnimationSet(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::AnimationSet;
		setName();
	}

	NodeAnimationSet() : NodeAnimationSet(true) {
	}
};

class NodeAnimationSetFrameTexture : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)
	PROPERTY_STRING_GETSET(Texture)
	PROPERTY_RECTINT_GETSET(TextureSourceRect)
	NodeAnimationSetFrameTexture(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::AnimationSetFrameTexture;
		setName();
		setTexture();
		setTextureSourceRect();
	}
	NodeAnimationSetFrameTexture() : NodeAnimationSetFrameTexture(true) {
	}
};

class NodeAnimationSetFrameTextureAtlas : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)
	PROPERTY_STRING_GETSET(TextureAtlasName)
	PROPERTY_STRING_GETSET(TextureAtlasFrameName)
	NodeAnimationSetFrameTextureAtlas(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::AnimationSetFrameTextureAtlas;
		setName();
		setTextureAtlasName();
		setTextureAtlasFrameName();
	}
	NodeAnimationSetFrameTextureAtlas() : NodeAnimationSetFrameTextureAtlas(true) {
	}
};




