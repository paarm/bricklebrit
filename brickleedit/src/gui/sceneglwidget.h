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
	const float minZ=-80;
	const float maxScaleMinusFactor=-0.999;
	const float maxZ=100;
	const float maxScalePlusFactor=8.0;
	float x=0;
    float y=0;
    float z=0;
	float scaleFactor=1.0;
	glm::mat4 viewMatrix;
    bool dirty=true;
public:
    void move(float distX, float distY, float distZ) {
        if (distX!=0.0 || distY!=0.0 || distZ!=0.0) {
            x+=distX;
            y+=distY;
			z+=distZ;
			if (z>maxZ) z=maxZ;
			if (z<minZ) z=minZ;
            dirty=true;
        }
    }
	float getScaleFactor() {
		return scaleFactor;
	}
	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	float getZ() {
		return z;
	}

    glm::mat4& getViewMatrix() {
        if (dirty) {
            viewMatrix=glm::mat4(1.0);
			viewMatrix=glm::translate(viewMatrix, glm::vec3(x,y,0.0));

			float sf=0.0;
			if (z>0.0) {
				sf=maxScalePlusFactor*(z/maxZ);
			} else if (z<0.0) {
				sf=maxScaleMinusFactor*(z/minZ);
				//if (sf<-1.0) sf=-1.0;
			}
			scaleFactor=1.0+sf;
			viewMatrix=glm::scale(viewMatrix, glm::vec3(scaleFactor,scaleFactor,1.0));
			dirty=false;
		}
        return viewMatrix;
    }
};

struct ViewportInfo {
	int viewportWidth=0;
	int viewportHeight=0;
	int orthoLeft=0;
	int orthoRight=0;
	int orthoBottom=0;
	int orthoTop=0;
};

struct ViewportMoveInfo {
	bool isOnMove=false;
	int startX=0;
	int startY=0;
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
