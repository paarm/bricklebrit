#pragma once
#include "mainwindow.h"
#include "../project/projectcontext.h"
#include "selectionmanager/selectionmanager.h"
#include <QObject>

enum class Tool {
	Selection,
	Brush
};

class GuiContext : public QObject
{
private:
	Q_OBJECT
	MainWindow*			mMainWindow=nullptr;
	NodeResource*		mCurrentResource=nullptr;
	NodeScene*			mCurrentScene=nullptr;
	SelectionManager	mSelectionManager;
	Tool				mCurrentTool;
	Node2d*				mCurrentPaintCanvas=nullptr;
	GuiContext();
	void sceneSwitched();
	void resourceSwitched();
	void projectSwitched();
public:
	static GuiContext& getInstance() {
		static GuiContext rGuiContext;
		return rGuiContext;
	}
	void setMainWindow(MainWindow* rMainWindow);
	MainWindow& getMainWindow();
	void setWindowTitle();

	bool createNewProject(const string& rProjectPathAbs, const string& rProjectName);
	bool loadProject(const string&rProjectPathWithFileAbs);

	bool createNewScene(const string& rSceneName, bool asDefault);
	void setCurrentScene(const string& rSceneName);
	NodeScene* getCurrentScene();

	bool createNewResource(const string& rName, bool asDefault);
	void setCurrentResource(const string& rResourceName);
	NodeResource* getCurrentResource();

	QString getVirtualProjectPath();
	QString fromVirtualPath(const QString &rPathAbs);
	QString toVirtualPath(const QString &rPathAbs);
	//void switchProperties(Node* rNode, NodeInfoType rNodeInfoType);


	void updateGlWidget();
	void currentPropertyValueChanged(Node* rNode);
	void updateChildNodes(Node *rNode_parent);
	void updateNodeName(Node *rNode);
	void insertNewSceneNode(Node *rNode);
	void insertNewResourceNode(Node *rNode);

	void setSceneNodeInTreeAsSelected(Node *rNode);

	SelectionManager& getSelectionManager();
	void onZoomLevelChanged(int rZoomLevel);

	Tool getCurrentTool();
	void setCurrentTool(Tool rTool);

	void setCurrentPaintCanvas(Node2d *rNode);
	Node2d* getCurrentPaintCanvas();
	GLMVector3 unprojectedScreenCoord(int mx, int my);
public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();
	void onSaveProjectClicked();
	void onCloseProjectClicked();
	void onNewSceneClicked();
	void onNewResourceClicked();
	void onToolSelectionActivated();
	void onToolBrushActivated();
	// Scene Tree
};

