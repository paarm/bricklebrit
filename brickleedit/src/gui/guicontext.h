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
public:
	static GuiContext& getInstance() {
		static GuiContext rGuiContext;
		return rGuiContext;
	}
	void setMainWindow(MainWindow* rMainWindow);
	MainWindow& getMainWindow();
	void setWindowTitle();

	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);

	bool createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs);
	bool loadCurrentScene(const string&rScenePathWithFileAbs);
	QString getVirtualProjectPath();
	QString fromVirtualPath(const QString &rPathAbs);
	QString toVirtualPath(const QString &rPathAbs);
public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();
	void onNewSceneClicked();
	void onOpenSceneClicked();
	// Scene Tree
	void onCreateNewNode(QString rNodeTypeName);
};

