#pragma once
#include <QtWidgets>
#include "scene/sceneglwidget.h"
#include "toolbar.h"
#include "menubar.h"
#include "newproject/newprojectdialog.h"
#include "newscene/newscenedialog.h"
#include "scenetree/scenetreedock.h"
#include "propertygrid/propertytreedock.h"
#include "brushmanager/brushdock.h"

class MainWindow : public QMainWindow
{
private:
	SceneGlWidget	*mSceneGlWidget=nullptr;
	MenuBar			*mMenuBar=nullptr;
	ToolBar			*mToolBar=nullptr;
	SceneTreeDock	*mSceneTreeDock=nullptr;
	PropertyTreeDock*mScenePropertyTreeDock=nullptr;
	BrushDock		*mBrushDock=nullptr;
	NewProjectDialog *mNewProjectDialog=nullptr;
	NewSceneDialog  *mNewSceneDialog=nullptr;
	NewSceneDialog *mNewResourceDialog=nullptr;

	QAction			*mActionNewProject=nullptr;
	QAction			*mActionOpenProject=nullptr;
	QAction			*mActionSaveProject=nullptr;
	QAction			*mActionCloseProject=nullptr;
	QAction			*mActionNewScene=nullptr;
	QAction			*mActionNewResource=nullptr;

	QAction			*mActionToolSelection=nullptr;
	QAction			*mActionToolBrush=nullptr;

	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	ToolBar& getToolBar();
	MenuBar& getMenuBar();
	NewProjectDialog& getNewProjectDialog();
	NewSceneDialog& getNewSceneDialog();
	NewSceneDialog& getNewResourceDialog();
	QAction*	getActionNewProject();
	QAction*	getActionOpenProject();
	QAction*	getActionSaveProject();
	QAction*	getActionCloseProject();
	QAction*	getActionNewScene();
	QAction*	getActionNewResource();

	QAction*	getActionToolSelection();
	QAction*	getActionToolBrush();

	SceneTreeDock& getSceneTreeDock();
	PropertyTreeDock& getPropertyTreeDock();
	BrushDock& getBrushDock();
	SceneGlWidget& getSceneGlWidget();

	void setProjectAvailable(bool isAvailable);
	void setSceneAvailable(bool isAvailable);
	void setProjectRequireSave(bool rRequireSave);
};

