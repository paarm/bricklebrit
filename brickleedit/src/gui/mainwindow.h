#pragma once
#include <QtWidgets>
#include "sceneglwidget.h"
#include "toolbar.h"
#include "newproject/newprojectdialog.h"
#include "newscene/newscenedialog.h"
#include "scenetree/scenetreedock.h"

class MainWindow : public QMainWindow
{
private:
	SceneGlWidget	*mSceneGlWidget=nullptr;
	ToolBar			*mToolBar=nullptr;
	SceneTreeDock	*mSceneTreeDock=nullptr;
	QDockWidget		*mElementPropertyDock=nullptr;
	NewProjectDialog *mNewProjectDialog=nullptr;
	NewSceneDialog  *mNewSceneDialog=nullptr;

	QAction			*mActionNewProject=nullptr;
	QAction			*mActionOpenProject=nullptr;
	QAction			*mActionNewScene=nullptr;
	QAction			*mActionOpenScene=nullptr;

	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	ToolBar& getToolBar();
	NewProjectDialog& getNewProjectDialog();
	NewSceneDialog& getNewSceneDialog();
	QAction*	getActionNewProject();
	QAction*	getActionOpenProject();
	QAction*	getActionNewScene();
	QAction*	getActionOpenScene();

	SceneTreeDock& getSceneTreeDock();
};

