#include "mainwindow.h"
#include <QApplication>
#include <QLayout>
#include <QFormLayout>
#include <myopenglwidget.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	MyOpenGlWidget gl(&w);

	w.resize(1280,960);
	w.show();

	return a.exec();
}
