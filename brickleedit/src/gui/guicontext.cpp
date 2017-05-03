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

void GuiContext::onNewProjectClicked() {
	getMainWindow().getNewProjectDialog().init();
	getMainWindow().getNewProjectDialog().show();
}

void GuiContext::onOpenProjectClicked() {
	QString file = QFileDialog::getOpenFileName(&getMainWindow(), tr("Open Project"),
												 "Projects",
												 "*.brprj",
												 nullptr,
												 QFileDialog::DontResolveSymlinks);

	if (!file.isEmpty()) {
		loadProject(file.toStdString());
		if (!ProjectContext::getInstance().loadProject(file.toStdString())) {
			QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Project not found"), QMessageBox::Ok);
		}
	}
}

void GuiContext::onNewSceneClicked() {
}
void GuiContext::onOpenSceneClicked() {
}

bool GuiContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().createNewProject(rProjectName, rProjectPathAbs, rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle(QString::fromStdString(ProjectContext::getInstance().getNodeProject()->getProjectName()));
	}
	return rv;
}

bool GuiContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().loadProject(rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle(QString::fromStdString(ProjectContext::getInstance().getNodeProject()->getProjectName()));
	}
	return rv;
}

