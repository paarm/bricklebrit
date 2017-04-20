#include "mainwindow/mainwindow.h"
#include "mainwindow/sceneglwidget.h"
#include <QApplication>
#include <QLayout>
#include <QFormLayout>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.resize(1280,960);
	w.show();

	return a.exec();
}
