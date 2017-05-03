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
	void setWindowTitle(const QString& rProjectName);

	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);
public slots:
	void onNewProjectClicked();
	void onOpenProjectClicked();
	void onNewSceneClicked();
	void onOpenSceneClicked();
};

