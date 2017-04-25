#pragma once
#include <QtWidgets>
#include "sceneglwidget.h"
#include "toolbar.h"
#include "newproject/newprojectdialog.h"
#include "scenetree/scenetreedock.h"

class MainWindow : public QMainWindow
{
private:
	SceneGlWidget	*mSceneGlWidget=nullptr;
	ToolBar			*mToolBar=nullptr;
	SceneTreeDock	*mSceneTreeDock=nullptr;
	QDockWidget		*mElementPropertyDock=nullptr;
	NewProjectDialog *mNewProjectDialog=nullptr;
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	ToolBar& getToolBar();
	NewProjectDialog& getNewProjectDialog();
};

