#pragma once
//#include <GL/glew.h>
//#include <GL/glext.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSizePolicy>
#include <QPainter>
#include <QOpenGLTexture>
#include <bricklelib.h>
#include "../project/projectcontext.h"
#include <QMouseEvent>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define mWidth 1280
#define mHeight 768

struct TVertex {
	float u;
	float v;
	float x;
	float y;
	float z;
};

struct ViewportInfo {
	int viewportWidth=0;
	int viewportHeight=0;
	int camaraOffsetX=mWidth/2;
	int camaraOffsetY=mHeight/2;
	int zoomLevelVirtual=0;
	int zoomLevel=0;
	float zoomFactor=1.0;

	int orthoLeft=0;
	int orthoRight=0;
	int orthoBottom=0;
	int orthoTop=0;
	bool updateCamaraOnDraw=false;
};

struct ViewportMoveInfo {
	bool isOnMove=false;
	int startX=0;
	int startY=0;
	int currentX=0;
	int currentY=0;
	int distanceX=0;
	int distanceY=0;
};

class SceneGlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
private:
	 void onMouseClicked(int mx, int my);
	 void setCurrentWordCoordinateToNode(Node2d *rNode2d, GLfloat *matrixArray);
	 void updateCamera();	 
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
protected:
	QImage *mImage;
	ViewportInfo mViewportInfo;
	ViewportMoveInfo mViewportMoveInfo;
    //QOpenGLTexture *texture;
	//BTexturePng bTexture;
	GLuint vbonum;
	void paintNode(Node* rNode);
	void paintGL();
signals:
	void zoomChanged(int rZoomLevel);
};
