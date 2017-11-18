#ifndef NODESPRITE_H
#define NODESPRITE_H
#include <list>
#include "node.h"
#include "../texture/btexturepng.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct TextureCoords {
	float u;
	float v;
};

struct WireVertex {
	float x;
	float y;
	float z;
};

struct ColorInfo {
	float r;
	float g;
	float b;
	bool operator==(ColorInfo &rColorInfoOther) {
		if (rColorInfoOther.r==r && rColorInfoOther.g==g && rColorInfoOther.b==b) {
			return true;
		}
		return false;
	}
	bool operator!=(ColorInfo &rColorInfoOther) {
		return !(&rColorInfoOther==this);
	}
};

struct WireVertexWithColor {
	ColorInfo	colorInfo;
	WireVertex wireVertext;
};

struct TextureVertex {
	struct TextureCoords	textureCoords;
	struct WireVertex		wireVertex;
};

struct TextureVertexBox {
	TextureVertex textureVertex[4];
};

struct WireVertexWithColorBox {
	WireVertexWithColor wireVertexWithColor[4];
};

struct VertexBufferBase {
	unsigned int		rCapacity=0;
	unsigned int		rUsed=0;
	unsigned int		rAboutToUse=0;
	unsigned int		rNextToWriteIndex=0;
	GLuint				rGlBufferId=0;
};

struct TexturedChildGroup : VertexBufferBase {
	BTexturePng			*rBTexturePng=nullptr;
	TextureVertexBox	*rVertexBuffer=nullptr;
};

struct WiredChildGroup : VertexBufferBase {
	WireVertexWithColorBox	*rVertexBuffer=nullptr;
};

struct DirtyInfo {
	bool rBTexturePng=false;
	bool rPosition=false;
	bool rSize=false;
	bool rScale=false;
	bool rTextureUV=false;
	bool rRotation=false;
	bool rRemoveScheduled=false;
	bool rIsNew=false;
	bool rVisible=false;
	bool rFlipX=false;
	bool rFlipY=false;
	bool		isDirty() {
		bool rv=false;
		if (rBTexturePng ||
				rPosition ||
				rSize ||
				rScale ||
				rTextureUV ||
				rRotation ||
				rRemoveScheduled ||
				rIsNew||
				rVisible||
				rFlipX ||
				rFlipY) {
			rv=true;
		}
		return rv;
	}
	void resetDirty() {
		rBTexturePng=false;
		rPosition=false;
		rSize=false;
		rScale=false;
		rTextureUV=false;
		rRotation=false;
		rRemoveScheduled=false;
		rIsNew=false;
		rVisible=false;
		rFlipX=false;
		rFlipY=false;
	}
};

namespace HandleLocation {
	enum HandleNumber {
		BottomRight=0,
		Right=1,
		Bottom=2
	};
}
struct HandleLocationInfo {
	//glm::vec4	rLocalLocationCenter;
	glm::vec4	rLocalLocationBox[4];

	//glm::vec4	rWorldLocationCenter;
	glm::vec4	rWorldLocationBox[4];

	glm::vec4	rVertexLocationBox[4];
};

struct LocationInfo {
	glm::vec3	rPosition;
	glm::vec2	rSize;
	glm::vec3	rScale;
	float		rRotation=0.0;
	glm::vec4	rLocalLocationCenter;
	glm::vec4	rLocalLocationBox[4];

	glm::vec4	rWorldLocationCenter;
	glm::vec4	rWorldLocationBox[4];

	glm::vec4	rVertexLocationBox[4];
	glm::mat4	rModelMatrixWithParent;
	glm::mat4	rModelMatrixWithParentInverse;
	glm::mat4	rModelMatrix;
	bool		rVisible=true;
	bool		rFlipX=false;
	bool		rFlipY=false;
};

struct TextureInfo {
	BTexturePng *rBTexturePng=nullptr;
	glm::vec2	rTextureXY;
	glm::vec2	rTextureWH;
};

struct FrameRefInfo {
	FrameRef	rFrameRef;
	//bool		rIsFrameDirty;
};

struct SelectionInfo {
	bool		rIsSelected=false;
	bool		rIsSelectionDirty=false;
	ColorInfo	rColorInfo={255,255,255};
};



class Node2d : public Node {
private:
	LocationInfo				mLocationInfo;
	HandleLocationInfo			mHandleLocationInfo[3];
	TextureInfo					mTextureInfo;
	FrameRefInfo				mFrameRefInfo;
	SelectionInfo				mSelectionInfo;
	DirtyInfo					mDirtyInfo;
	list<TexturedChildGroup*>	*mTexturedChildGroupList=nullptr;
	WiredChildGroup				*mWiredChildGroup=nullptr;
	WiredChildGroup				*mHandleWiredChildGroup=nullptr;

	void cleanup();
	void deleteTexturedChildGroupList();
	WiredChildGroup* deleteXWiredChildGroup(WiredChildGroup *rWiredChildGroup);
	bool reallocateWiredVertexBufferIfRequired(WiredChildGroup* rWiredChildGroup, bool haveDirty);

	void updateVisible(bool rVisible);
	void updatePosition(float x, float y, float z);
	void updateSize(float x, float y);
	void updateScale(float x, float y);
	void updateRotation(float rRotation);
	void updateFlipX(bool rFlipX);
	void updateFlipY(bool rFlipY);
	void updateFrameRef(const string&resourcefile, int textureid, const string&frame);
	void updateTextureAll();
	void updateTexture(BTexturePng *rBTexturePng);
	void updateTextureUV(float x, float y, float u, float v);


	void updateMatrix(bool forceDirtyParent);
	void updateTextureVertexBuffer(bool forceDirtyParent);
	void updateSelectionVertexBuffer(bool forceDirtyParent);
	void draw(glm::mat4 &rViewMatrix);
	void drawSelection(glm::mat4 &rViewMatrix);
	glm::mat4& getModelMatrixWithParents();
	glm::mat4& getModelMatrixWithParentsInverse();
	glm::mat4& getModelMatrix();



	PointFloat mCurrentWorldLocationCenter;
	PointFloat mCurrentWorldLocationBox[4];

	PointFloat mCurrentLocalLocationCenter;
	PointFloat mCurrentLocalLocationBox[4];

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
	GLMMatrix4 mCurrentLocalModelMatrix;
	float z;
public:
	virtual ~Node2d();
#if 0
	PROPERTY_POINTINT_GETSET(Position)
	PROPERTY_POINTINT_GETSET(Size)
	PROPERTY_POINTFLOAT_GETSET(Scale)
	PROPERTY_FLOAT_GETSET(Rotation)
	PROPERTY_BOOL_GETSET(FlipX)
	PROPERTY_BOOL_GETSET(FlipY)
	PROPERTY_BOOL_GETSET(Visible)
#endif
	Node2d(bool rCreateNewId) : Node(rCreateNewId) {
		mNodeType=NodeType::Node2d;
		setPosition();
		setSize();
		setScale(PointFloat(1.0,1.0));
		setRotation();
		setFrameRef();

		setFlipX(false);
		setFlipY(false);
		setVisible(true);
		mIsDrawable=true;
	}
	virtual void initialSetupAfterLoadRecursive() override;
	void synchronizeProperties();

	void setPosition(const PointInt& r);
	void setPosition();
	PointInt& getPosition();
	//Size
	void setSize(const PointInt& r);
	void setSize();
	PointInt& getSize();
	//Scale
	void setScale(const PointFloat& r);
	void setScale();
	PointFloat& getScale();
	//Rotation
	void setRotation(const float& r);
	void setRotation();
	float& getRotation();
	//FlipX
	void setFlipX(const bool& r);
	void setFlipX();
	bool& getFlipX();
	//FlipY
	void setFlipY(const bool& r);
	void setFlipY();
	bool& getFlipY();
	//Visible
	void setVisible(const bool& r);
	void setVisible();
	bool& getVisible();
	//FrameRef
	void setFrameRef(const FrameRef& r);
	void setFrameRef();
	FrameRef& getFrameRef();

	void updateDraw(glm::mat4 &rViewMatrix);
	void updateSelection(bool isSelected, ColorInfo rColorInfo);
	void pickUpBox(vector<Node2d*> &v, float x, float y, float dx, float dy, bool onlyIfFullInside);
	HandleLocationInfo* intersectsHandle(float worldX, float worldY, HandleLocation::HandleNumber rHandleNumber);

	LocationInfo& getLocationInfo();
	glm::vec4 getLocalPosFromWorldPos(float rWorldX, float rWorldY, bool rSnapToGrid);
	float calcGridPos(float worldPos, float gridSize);
	void snapToGrid(glm::vec4 &v);

	//void clearSelectionRecursive();

#if 0
	void setCurrentModelMatrix(GLMMatrix4 &m);
	GLMMatrix4 getCurrentModelMatrix();

	void setCurrentLocalModelMatrix(GLMMatrix4 &m);
	GLMMatrix4 getCurrentLocalModelMatrix();
#endif
	void setResizeHandleSizeLocal(float rResizeHandleSizeLocalX, float rResizeHandleSizeLocalY);
	//void calculateCoords(GLMMatrix4 &m, PointFloat* current2LocationCenter, PointFloat* mCurrent4LocationBox);

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
#if 0
	PointFloat &getCurrentWorldLocationCenter() {
		return mCurrentWorldLocationCenter;
	}
	PointFloat *getCurrentWorldLocationBox() {
		return mCurrentWorldLocationBox;
	}

	PointFloat *getCurrentLocalLocationBox() {
		return mCurrentLocalLocationBox;
	}
#endif
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
	PROPERTY_BOOL_GETSET(Locked)

	NodeLayer(bool rCreateNewId) : Node2d(rCreateNewId) {
		mNodeType=NodeType::Layer;
		setLocked(false);
	}

	NodeLayer() : NodeLayer(true) {
	}
};

class NodeSprite : public Node2d {
private:
public:
	//PROPERTY_BOOL_GETSET(IsAnimated)
	// if isAnimated==true
		//PROPERTY_FRAMEREF_GETSET(FrameRef)
	// else
		//PROPERTY_STRING_GETSET(DefaultAnimation)
	// end if
	NodeSprite(bool rCreateNewId) : Node2d(rCreateNewId) {
		mNodeType=NodeType::Sprite;
		//setIsAnimated(false);
		//setFrameRef();
		//setDefaultAnimation();
	}

	NodeSprite() : NodeSprite(true) {
	}
};

#endif // NODESPRITE_H
