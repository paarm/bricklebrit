#pragma once
#include "mainwindow.h"
#include "../project/projectcontext.h"
#include <QObject>
class GuiContext : public QObject
{
private:
	Q_OBJECT
	MainWindow *mMainWindow=nullptr;
	vector<Node*>	mSelectedSceneNodes;
	NodeResource * mCurrentResource=nullptr;
	NodeScene * mCurrentScene=nullptr;
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

	NodeSceneInfo* getSceneInfoByName(const string& rSceneName);
	NodeScene* getOrLoadSceneByName(const string& rSceneName);
	bool createNewScene(const string& rSceneName, bool asDefault);
	void setCurrentScene(const string& rSceneName);
	NodeScene* getCurrentScene();

	NodeResourceInfo* getResourceInfoByName(const string& rResourceName);
	NodeResource* getOrLoadResourceByName(const string& rResourceName);
	bool createNewResource(const string& rName, bool asDefault);
	void setCurrentResource(const string& rResourceName);
	NodeResource* getCurrentResource();

	QString getVirtualProjectPath();
	QString fromVirtualPath(const QString &rPathAbs);
	QString toVirtualPath(const QString &rPathAbs);
	void switchProperties(Node* rNode, NodeInfoType rNodeInfoType);
	void updateGlWidget();
	void currentPropertyValueChanged(Node* rNode, NodeInfoType rNodeInfoType);
	void updateChildNodes(Node *rNode_parent, NodeInfoType rNodeInfoType);

	void clearSelectedSceneNodes();
	void clearSelectedSceneNode(Node *rDeselectedNode);
	void replaceSelectedSceneNode(Node *rSelectedNode);
	bool isSceneNodeSelected(Node*);
	void setSceneNodeAsSelected(Node*);
public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();
	void onSaveProjectClicked();
	void onCloseProjectClicked();
	void onNewSceneClicked();
	void onNewResourceClicked();
	// Scene Tree
	void onCreateNewNode(QString rNodeTypeName, NodeInfoType rNodeInfoType);
};

