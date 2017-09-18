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

class Camara {
private:
    float x=0;
    float y=0;
    float z=0;
    glm::mat4 viewMatrix;
    bool dirty=true;
public:
    void move(float distX, float distY, float distZ) {
        if (distX!=0.0 || distY!=0.0 || distZ!=0.0) {
            x+=distX;
            y+=distY;
            z+=distZ;
            dirty=true;
        }
    }

    glm::mat4& getViewMatrix() {
        if (dirty) {
            viewMatrix=glm::mat4(1.0);
            viewMatrix=glm::translate(viewMatrix, glm::vec3(x,y,z));
        }
        return viewMatrix;
    }
};

struct ViewportInfo {
	int viewportWidth=0;
	int viewportHeight=0;
    int camaraOffsetX=0;//mWidth/2;
    int camaraOffsetY=0;//mHeight/2;
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
protected:
	QImage *mImage;
	ViewportInfo mViewportInfo;
    Camara mCamara;
	ViewportMoveInfo mViewportMoveInfo;
    //QOpenGLTexture *texture;
	//BTexturePng bTexture;
	GLuint vbonum;
	void paintNode(Node* rNode);
	void paintGL();
signals:
	void zoomChanged(int rZoomLevel);
};
