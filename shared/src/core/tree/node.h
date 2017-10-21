#pragma once

#include <vector>
#include <map>
#include <string>
#include "property/property.h"
#include "../jsonparser/jsonparser.h"
#include "nodeidgenerator.h"
#include <future>
#include "../math/glmwrapper.h"


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
	Animation,
	AnimationFrame,
	Project,
	ResourceInfo,
	SceneInfo,
	Scene,
	Layer,
	SceneRef,


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
    void deleteChildNode(Node *rNodeToDelete);
    void deleteNode(Node *rNodeToDelete);
	unsigned long getChildCount();
	Node* getNodeFromIndex(unsigned long rIndex);
	Node* getNodeWithNodeId(int rNodeId);
    Node* getChildNodeWithNodeIdRecursive(int rNodeId);
    Node* getChildNodeWithName(const string &rName);
	Node* getChildNodeWithNameAndNodeType(const string &rName, NodeType rNodeType);
	vector<Node*> getChildNodesWithNodeType(NodeType rNodeType);

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
Node * getInstanceFromNodeType(NodeType rNodeType, bool rCreateNewId);


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
	PROPERTY_BOOL_GETSET(GridActive)
	PROPERTY_POINTINT_GETSET(GridSize)
	PROPERTY_POINTINT_GETSET(GridOffset)



	NodeProject(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Project;
		setScenesSubPath("");
		setResourcesSubPath("");
		setStartScene("");
		setProjectResource("");
		setNextFreeId(0);
		setListTest();
		setGridActive(false);
		setGridSize();
		setGridOffset();
	}

	NodeProject() : NodeProject(true) {
	}
};


class Node2d : public Node {
private:
	PointFloat mCurrentWorldLocationCenter;
	PointFloat mCurrentWorldLocationBox[4];

	float	   mResizeHandleSizeLocal;
	PointFloat mResizeHandleBR[4];
	PointFloat mResizeHandleLocalBR[4];
	PointFloat mResizeHandleRight[4];
	PointFloat mResizeHandleLocalRight[4];
	PointFloat mResizeHandleBottom[4];
	PointFloat mResizeHandleLocalBottom[4];

	//PointFloat mResizeHandleR[4];
	//PointFloat mResizeHandleB[4];
	GLMMatrix4 mCurrentModelMatrix;
	float z;
public:
	PROPERTY_POINTINT_GETSET(Position)
	PROPERTY_POINTINT_GETSET(Size)
	PROPERTY_POINTFLOAT_GETSET(Scale)
	PROPERTY_FLOAT_GETSET(Rotation)
	PROPERTY_BOOL_GETSET(FlipX)
	PROPERTY_BOOL_GETSET(FlipY)

	Node2d(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Node2d;
		setPosition();
		setSize();
		setScale(PointFloat(1.0,1.0));
		setRotation();
		setFlipX(false);
		setFlipY(false);
	}

	void setCurrentModelMatrix(GLMMatrix4 &m);
	GLMMatrix4 getCurrentModelMatrix();

	void setResizeHandleSizeLocal(float rResizeHandleSizeLocalX, float rResizeHandleSizeLocalY);

	PointFloat* getResizeHandleBottomRight() {
		return mResizeHandleBR;
	}
	PointFloat* getResizeHandleRight() {
		return mResizeHandleRight;
	}
	PointFloat* getResizeHandleBottom() {
		return mResizeHandleBottom;
	}
	PointFloat* getResizeHandleLocalBottomRight() {
		return mResizeHandleLocalBR;
	}
	PointFloat* getResizeHandleLocalRight() {
		return mResizeHandleLocalRight;
	}

	PointFloat *getResizeHandleLocalBottom() {
		return mResizeHandleLocalBottom;
	}

	PointFloat &getCurrentWorldLocationCenter() {
		return mCurrentWorldLocationCenter;
	}
	PointFloat *getCurrentWorldLocationBox() {
		return mCurrentWorldLocationBox;
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

class NodeLayer : public Node2d {
private:
public:

	NodeLayer(bool rCreateNewId) : Node2d(rCreateNewId) {
		mNodeType=NodeType::Layer;
	}

	NodeLayer() : NodeLayer(true) {
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

class NodeResourceInfo : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Path)
	PROPERTY_BOOL_GETSET(IsDefault)
	NodeResourceInfo(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::ResourceInfo;
		setPath("");
		setIsDefault(false);
	}
	NodeResourceInfo() : NodeResourceInfo(true) {
	}
};

class NodeSceneInfo : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Path)
	PROPERTY_BOOL_GETSET(IsDefault)
	NodeSceneInfo(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::SceneInfo;
		setPath("");
		setIsDefault(false);
	}
	NodeSceneInfo() : NodeSceneInfo(true) {
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



class NodeAnimation : public Node {
private:
public:
	PROPERTY_STRING_GETSET(Name)

	NodeAnimation(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Animation;
		setName();
	}

	NodeAnimation() : NodeAnimation(true) {
	}
};

class NodeAnimationFrame : public Node {
private:
public:
	PROPERTY_FRAMEREF_GETSET(FrameRef)

	NodeAnimationFrame(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::AnimationFrame;
		setFrameRef();
	}

	NodeAnimationFrame() : NodeAnimationFrame(true) {
	}
};







