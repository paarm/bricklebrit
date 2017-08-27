#pragma once
#include "mainwindow.h"
#include "../project/projectcontext.h"
#include <QObject>
class GuiContext : public QObject
{
private:
	Q_OBJECT
	MainWindow *mMainWindow=nullptr;
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

	bool createNewScene(const string& rSceneName);
	bool createNewResource(const string& rName);
	bool loadCurrentScene(const string&rName);
	bool loadCurrentResource(const string&rName);

	QString getVirtualProjectPath();
	QString fromVirtualPath(const QString &rPathAbs);
	QString toVirtualPath(const QString &rPathAbs);
	void switchProperties(Node* rNode, NodeInfoType rNodeInfoType);
public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();
	void onSaveProjectClicked();
	void onCloseProjectClicked();
	void onNewSceneClicked();
	void onOpenSceneClicked();
	void onNewResourceClicked();
	void onOpenResourceClicked();
	// Scene Tree
	void onCreateNewNode(QString rNodeTypeName, NodeInfoType rNodeInfoType);
};

