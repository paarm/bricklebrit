#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sceneglwidget.h"
#include <QToolBar>
#include <QDockWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSpacerItem>
#include <QTreeWidget>
#include <QSizePolicy>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	SceneGlWidget*	mSceneGlWidget=nullptr;
	QToolBar*		mToolBar=nullptr;
	QDockWidget*	mTreeDock=nullptr;
	QDockWidget*	mPropertiesDock=nullptr;
};

#endif // MAINWINDOW_H
