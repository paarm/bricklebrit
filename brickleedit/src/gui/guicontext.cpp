#include "guicontext.h"
#include <QMessageBox>

GuiContext::GuiContext()
{

}

MainWindow& GuiContext::getMainWindow() {
	return *mMainWindow;
}

void GuiContext::setMainWindow(MainWindow* rMainWindow) {
	mMainWindow=rMainWindow;
}

void GuiContext::setWindowTitle(const QString& rProjectName) {
	getMainWindow().setWindowTitle("Brickleedit - "+rProjectName);
}

void GuiContext::onNewProject() {
	getMainWindow().getNewProjectDialog().init();
	getMainWindow().getNewProjectDialog().show();
}

void GuiContext::onOpenProject() {
	QString file="/home/pam/Programmierung/bricklebrit/brickleedit/bin/Projects/NewTest/brickleroot.txt";
	if (!ProjectContext::getInstance().loadProject(file.toStdString())) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Project not found"), QMessageBox::Ok);
	} else {
		GuiContext::getInstance().setWindowTitle(QString::fromStdString(ProjectContext::getInstance().getNodeProject()->getProjectName()));
	}
}
