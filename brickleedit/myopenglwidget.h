#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSizePolicy>
//#include <GL/glu.h>

 class MyOpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	MyOpenGlWidget(QWidget *parent) : QOpenGLWidget(parent) {
		QSurfaceFormat format;
		format.setDepthBufferSize(24);
/*		format.setStencilBufferSize(8);
		format.setVersion(3, 2);
		format.setProfile(QSurfaceFormat::CoreProfile);
		*/
		setFormat(format);
		resize(1280,768);
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

	void resizeGL(int w, int h)
	{
		//glViewport(0,0,1,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);
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


#endif // MYOPENGLWIDGET_H
