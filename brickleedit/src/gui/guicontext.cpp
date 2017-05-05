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


void GuiContext::setWindowTitle() {
	string rPartTitle="";
	if (ProjectContext::getInstance().getNodeProject()) {
		rPartTitle+=" - "+ProjectContext::getInstance().getNodeProject()->getProjectName();
		if (ProjectContext::getInstance().getNodeCurrentScene()) {
			rPartTitle+=" : "+ProjectContext::getInstance().getNodeCurrentScene()->getName();
		}
	}
	getMainWindow().setWindowTitle("Brickleedit"+QString::fromStdString(rPartTitle));
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
		if (!loadProject(file.toStdString())) {
			QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Project not found"), QMessageBox::Ok);
		}
	}
}

void GuiContext::onNewSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		getMainWindow().getNewSceneDialog().setScenePath(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()));
		getMainWindow().getNewSceneDialog().setSceneName("New Scene");
		getMainWindow().getNewSceneDialog().show();
	}
}

void GuiContext::onOpenSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString file = QFileDialog::getOpenFileName(&getMainWindow(), tr("Open Scene"),
													 QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()),
													 "*.brscn",
													 nullptr,
													 QFileDialog::DontResolveSymlinks);

		if (!file.isEmpty()) {
			if (!file.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
				QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Scene is not inside the Project directory"), QMessageBox::Ok);
			} else {
				if (!loadCurrentScene(file.toStdString())) {
					QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Scene not found"), QMessageBox::Ok);
				}
			}
		}
	}
}

bool GuiContext::loadCurrentScene(const string&rScenePathWithFileAbs) {
	bool rv=ProjectContext::getInstance().loadCurrentScene(rScenePathWithFileAbs);
	if (rv) {
		setWindowTitle();
	}
	return rv;
}


bool GuiContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().createNewProject(rProjectName, rProjectPathAbs, rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle();
	}
	return rv;
}

bool GuiContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().loadProject(rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle();
	}
	return rv;
}

bool GuiContext::createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs) {
	bool rv=ProjectContext::getInstance().createNewScene(rSceneName, rScenePathAbs, rScenePathWithFileAbs);
	if (rv) {
		setWindowTitle();
	}
	return rv;
}

