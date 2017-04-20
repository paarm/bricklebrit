#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSizePolicy>
//#include <GL/glu.h>

 class SceneGlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	SceneGlWidget(QWidget *parent) : QOpenGLWidget(parent) {
		QSurfaceFormat format;
		format.setDepthBufferSize(24);
/*		format.setStencilBufferSize(8);
		format.setVersion(3, 2);
		format.setProfile(QSurfaceFormat::CoreProfile);
		*/
		setFormat(format);
		//resize(1280,768);
		//this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

protected:
	void initializeGL()
	{
		initializeOpenGLFunctions();
		glClearColor(0,0,0,1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
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

	void paintGL()
	{
		// Draw the scene:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_QUADS);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3d(-5,-5,0);
			glVertex3d(5,-5,0);
			glVertex3d(5,5,0);
			glVertex3d(-5,5,0);
		glEnd();

		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 0.0);
			glVertex3d(0,0,0);
			glVertex3d(mWidth,0,0);
			glVertex3d(mWidth,mHeight,0);
			glVertex3d(0,mHeight,0);
		glEnd();
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		//resize(1280,768);


		glBegin(GL_TRIANGLES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(-50, -50, 0);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f( 50, -50, 0);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f( 0.0,  50, 0);
		glEnd();
	}

};
