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

public slots:
	void onNewProject();
	void onOpenProject();
	void onNewScene();
	void onOpenScene();
};

