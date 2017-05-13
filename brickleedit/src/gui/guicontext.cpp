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

QString GuiContext::getVirtualProjectPath() {
	return "project://";
}

QString GuiContext::toVirtualPath(const QString &rPathAbs) {
	QString rv;
	if (rPathAbs.startsWith(getVirtualProjectPath())) {
		rv=rPathAbs;
	} else if (rPathAbs.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
		QString fragment=rPathAbs.mid(ProjectContext::getInstance().getProjectPathAbs().length(), rPathAbs.length()-ProjectContext::getInstance().getProjectPathAbs().length());
		if (fragment.length()>0 && (fragment.at(0)=='/' || fragment.at(0)=='\\')) {
			fragment=fragment.mid(1,fragment.length()-1);
		}
		rv+=getVirtualProjectPath()+fragment;
	}
	return rv;
}

QString GuiContext::fromVirtualPath(const QString &rPathAbs) {
	QString rv;
	if (rPathAbs.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
		rv=rPathAbs;
	} else if (rPathAbs.startsWith(getVirtualProjectPath())) {
		rv+=QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs());
		QString fragment=rPathAbs.mid(getVirtualProjectPath().length(), rPathAbs.length()-getVirtualProjectPath().length());
		if (fragment.length()>0 && fragment[0]!='/' && fragment[0]!='\\') {
			rv+='/'; // TODO windows
		}
		rv+=fragment;
	}
	return rv;
}



void GuiContext::setWindowTitle() {
	string rPartTitle="";
	if (ProjectContext::getInstance().getNodeProject()) {
		rPartTitle+=ProjectContext::getInstance().getNodeProject()->getProjectName();
		if (ProjectContext::getInstance().getNodeCurrentScene()) {
			rPartTitle+=" ["+ProjectContext::getInstance().getNodeCurrentScene()->getName()+"]";
		}
		rPartTitle+=" - ";
	}
	getMainWindow().setWindowTitle(QString::fromStdString(rPartTitle)+"Brickleedit");
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
		if (ProjectContext::getInstance().isCurrentSceneAvailable()) {
			getMainWindow().getNewSceneDialog().setScenePath(toVirtualPath(QString::fromStdString(ProjectContext::getInstance().getCurrentScenePathAbs())));
		} else {
			getMainWindow().getNewSceneDialog().setScenePath(getVirtualProjectPath());
		}
		getMainWindow().getNewSceneDialog().setSceneName("New Scene");
		getMainWindow().getNewSceneDialog().show();
	}
}

void GuiContext::onOpenSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString path=getVirtualProjectPath();
		if (ProjectContext::getInstance().isCurrentSceneAvailable()) {
			path=QString::fromStdString(ProjectContext::getInstance().getCurrentScenePathAbs());
		} else {
			getMainWindow().getNewSceneDialog().setScenePath(getVirtualProjectPath());
		}

		QString file = QFileDialog::getOpenFileName(&getMainWindow(), tr("Open Scene"),
													path,
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
		getMainWindow().getSceneTreeDock().clear();
		NodeScene* rNodeScene=ProjectContext::getInstance().getNodeCurrentScene();
		if (rNodeScene) {
			getMainWindow().getSceneTreeDock().addNode(nullptr, rNodeScene);
		}
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

void GuiContext::onCreateNewNode(QString rNodeTypeName) {
	QTreeWidgetItem *parent=getMainWindow().getSceneTreeDock().getSelectedItem();
	NodeType rNodeType=getNodeTypeFromString(rNodeTypeName.toStdString());
	Node*rNode=getInstanceFromNodeType(rNodeType, true);
	if (rNode) {
		rNode->setName(rNodeTypeName.toStdString());
		if (parent) {
			Node* parentNode=getMainWindow().getSceneTreeDock().getNodeFromTreeItem(parent);
			if (parentNode) {
				parentNode->addChildNode(rNode);
			}
		}
		getMainWindow().getSceneTreeDock().addNode(parent, rNode);
	}
}

