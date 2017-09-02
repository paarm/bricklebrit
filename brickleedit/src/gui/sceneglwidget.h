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

struct TVertex {
	float u;
	float v;
	float x;
	float y;
	float z;
};

 class SceneGlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	SceneGlWidget(QWidget *parent) : QOpenGLWidget(parent) {
		//QSurfaceFormat format;
		//format.setDepthBufferSize(24);
		//setFormat(format);


/*		format.setStencilBufferSize(8);
		format.setVersion(3, 2);
		format.setProfile(QSurfaceFormat::CoreProfile);
		*/
		//resize(1280,768);
		//this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}
	virtual ~SceneGlWidget() {

	}

protected:
	QImage *mImage;
    //QOpenGLTexture *texture;
	//BTexturePng bTexture;
	GLuint vbonum;
	void paintNode(Node* rNode);
	void paintGL();
	void initializeGL()
	{
		initializeOpenGLFunctions();
		glClearColor(0,0,0,1);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //texture= new QOpenGLTexture(QImage("data/assets/test.bmp"));

		//bTexture.load("data/assets/gravity.png");
#if 0
		glEnable(GL_TEXTURE_2D);
		unsigned error = lodepng::decode(image, lwidth, lheight, "data/assets/testhg.png");
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lwidth, lheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lwidth, lheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
#if 0
		QPainter p;
		p.begin(this);
		p.drawText(100,100,"Hallo Welt");
		p.end();
#endif

	}

#define mWidth 1280
#define mHeight 768
	void resizeGL(int w, int h)
	{
		//glViewport(0,0,1,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-w+(mWidth/2.0), w+(mWidth/2.0), h+(mHeight/2.0), -h+(mHeight/2.0), 0.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
#if 0
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, (float)w/h, 0.01, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0,0,5,0,0,0,0,1,0);
#endif
		// Update projection matrix and other size related settings:
		//m_projection.setToIdentity();
		//m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
	}

};
