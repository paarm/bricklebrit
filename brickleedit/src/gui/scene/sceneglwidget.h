#pragma once
//#include <GL/glew.h>
//#include <GL/glext.h>

#include <bricklelib.h>
//#include <glm/mat4x4.hpp>

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
#include <QSizePolicy>
#include <QPainter>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include "sceneitemmovemanager.h"
#include "sceneitemresizemanager.h"
#include "scenebulkselectmanager.h"

#if 0
struct TVertex {
	float u;
	float v;
	float x;
	float y;
	float z;
};
#endif
struct ViewportMoveInfo {
	bool isOnMove=false;
	int startX=0;
	int startY=0;
};

struct SceneMouseInfo {
	bool isInside=false;
	int x=0;
	int y=0;
};

#if 0
struct DrawMember {
	NodeSprite		*rNodeSprite=nullptr;
	NodeTextureFrame *rNodeTextureFrame=nullptr;
	DrawMember(NodeSprite *nodeSprite, NodeTextureFrame *nodeTextureFrame) : rNodeSprite(nodeSprite), rNodeTextureFrame(nodeTextureFrame) {

	}
};

struct DrawTexture {
	NodeTexture *rNodeTexture=nullptr;
	BTexturePng *bTexture=nullptr;
	vector<DrawMember> rDrawMemberList;
	DrawTexture(NodeTexture *nodeTexture, BTexturePng *texture) : rNodeTexture(nodeTexture), bTexture(texture) {}
};

struct VertexEntry {
	float s;
	float t;
	float x;
	float y;
	float z;
};
#endif

class SceneGlWidget : public QOpenGLWidget //, protected QOpenGLFunctions
{
private:
	int mVirtualWidth=1280;
	int mVirtualHeight=768;
	bool mCameraInitialized=false;
	 void onLeftMouseClicked(QMouseEvent *event, int mx, int my);
	 void updateViewport();
public:
	SceneGlWidget(QWidget *parent) : QOpenGLWidget(parent) {}
	virtual ~SceneGlWidget() {}

	void mousePressEvent(QMouseEvent * event ) override;
	void mouseReleaseEvent(QMouseEvent * event ) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void initializeGL();
	void resizeGL(int w, int h);
	void zoomInOut(int units);
	GLMVector3 unprojectedScreenCoord(int mx, int my);
	Camera& getCamera();
	void resetScene();
protected:
	//void hoverMove(QHoverEvent *event);
	bool event(QEvent *e) override;
	QImage *mImage;
	Camera mCamera;
	ViewportMoveInfo mViewportMoveInfo;
	SceneItemMoveManager	mSceneItemMoveManager;
	SceneItemResizeManager	mSceneItemResizeManager;
	SceneBulkSelectManager	mSceneBulkSelectManager;
	SceneMouseInfo	mSceneMouseInfo;
	//DrawNode		mDrawNodeScene;
    //QOpenGLTexture *texture;
	//BTexturePng bTexture;
	GLuint vbonum;
	//void paintNode(Node* rNode, bool isBrushCanvas);
	//void paintChilds(Node2d *rNodeParent);
	//void getTexture(NodeSprite *paintNode, NodeTexture **rOutNodeTexture, NodeTextureFrame **rOutNodeTextureFrame);
	//void getTextureCoords(NodeSprite* rNodeSprite, NodeTextureFrame *rNodeTextureFrame, BTexturePng *bTexture, float *outtx, float *outty, float *outtw, float* outth);
	//void initializeDrawNode(Node2d *rNodeParent);



	void paintGL();
signals:
	void zoomChanged(int rZoomLevel);
};
