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
	if (ProjectContext::getInstance().isProjectAvailable()) {
		rPartTitle+=ProjectContext::getInstance().getNodeProject()->getName();
		if (ProjectContext::getInstance().isSceneAvailable()) {
			rPartTitle+=" ["+ProjectContext::getInstance().getNodeScene()->getName()+"]";
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

void GuiContext::onCloseProjectClicked() {
	if (ProjectContext::getInstance().getNodeProject()) {
		ProjectContext::getInstance().close(true);
		projectSwitched();
		setWindowTitle();
	}
}

void GuiContext::onSaveProjectClicked() {
	if (ProjectContext::getInstance().getNodeProject()) {
		ProjectContext::getInstance().save();
	}
}

void GuiContext::onNewSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		if (ProjectContext::getInstance().isSceneAvailable()) {
			getMainWindow().getNewSceneDialog().setScenePath(toVirtualPath(QString::fromStdString(ProjectContext::getInstance().getScenePathAbs())));
		} else {
			getMainWindow().getNewSceneDialog().setScenePath(getVirtualProjectPath());
		}
		getMainWindow().getNewSceneDialog().setName("New Scene");
		getMainWindow().getNewSceneDialog().show();
	}
}

void GuiContext::onOpenSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString path=getVirtualProjectPath();
		if (ProjectContext::getInstance().isSceneAvailable()) {
			path=QString::fromStdString(ProjectContext::getInstance().getScenePathAbs());
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
	bool rv=ProjectContext::getInstance().load(NodeInfoType::Scene, rScenePathWithFileAbs);
	if (rv) {
		setWindowTitle();
		sceneSwitched();
	}
	return rv;
}

void GuiContext::onOpenResourceClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString path=getVirtualProjectPath();
		//if (ProjectContext::getInstance().isSceneAvailable()) {
		//	path=QString::fromStdString(ProjectContext::getInstance().getScenePathAbs());
		//} else {
		//	getMainWindow().getNewSceneDialog().setScenePath(getVirtualProjectPath());
		//}
		getMainWindow().getNewResourceDialog().setScenePath(getVirtualProjectPath());

		QString file = QFileDialog::getOpenFileName(&getMainWindow(), tr("Open Resource"),
													path,
													 "*.brres",
													 nullptr,
													 QFileDialog::DontResolveSymlinks);

		if (!file.isEmpty()) {
			if (!file.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
				QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Resource is not inside the Project directory"), QMessageBox::Ok);
			} else {
				if (!loadCurrentResource(file.toStdString())) {
					QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Resource not found"), QMessageBox::Ok);
				}
			}
		}
	}
}

bool GuiContext::loadCurrentResource(const string&rPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().load(NodeInfoType::Resource, rPathWithFileAbs);
	NodeResource *rNodeResource=nullptr;
	if (rv) {
		rNodeResource=ProjectContext::getInstance().getNodeResourceByPath(rPathWithFileAbs);
	}
	ProjectContext::getInstance().setCurrentResource(rNodeResource);
	resourceSwitched();
	return rv;
}

void GuiContext::onNewResourceClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		//if (ProjectContext::getInstance().isSceneAvailable()) {
		//	getMainWindow().getNewSceneDialog().setScenePath(toVirtualPath(QString::fromStdString(ProjectContext::getInstance().getScenePathAbs())));
		//} else {
		//}
		getMainWindow().getNewResourceDialog().setScenePath(getVirtualProjectPath());
		getMainWindow().getNewResourceDialog().setName("New Resource");
		getMainWindow().getNewResourceDialog().show();
	}
}


bool GuiContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().createNew(NodeInfoType::Project, rProjectName, rProjectPathAbs, rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle();
		projectSwitched();
	}
	return rv;
}

bool GuiContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=ProjectContext::getInstance().load(NodeInfoType::Project, rProjectPathWithFileAbs);
	if (rv) {
		setWindowTitle();
		projectSwitched();
	}
	return rv;
}

bool GuiContext::createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs) {
	bool rv=ProjectContext::getInstance().createNew(NodeInfoType::Scene, rSceneName, rScenePathAbs, rScenePathWithFileAbs);
	if (rv) {
		setWindowTitle();
		sceneSwitched();
	}
	return rv;
}

bool GuiContext::createNewResource(const string& rName, const string& rPathAbs, const string&rPathWithFileAbs) {
	bool rv=true;
	NodeResource *rNodeResource=ProjectContext::getInstance().getNodeResourceByPath(rPathWithFileAbs);
	if (rNodeResource==nullptr) {
		rv=ProjectContext::getInstance().createNew(NodeInfoType::Resource, rName, rPathAbs, rPathWithFileAbs);
		if (rv) {
			rNodeResource=ProjectContext::getInstance().getNodeResourceByPath(rPathWithFileAbs);
		}
	}
	ProjectContext::getInstance().setCurrentResource(rNodeResource);
	resourceSwitched();

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
		getMainWindow().getSceneTreeDock().addSceneNode(parent, rNode);
	}
}

void GuiContext::projectSwitched() {
	getMainWindow().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	getMainWindow().setProjectRequireSave(ProjectContext::getInstance().getNodeProject()); // TODO own mechanism
	getMainWindow().getSceneTreeDock().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	sceneSwitched();
}

void GuiContext::sceneSwitched() {
	getMainWindow().getSceneTreeDock().clearScene();
	NodeScene* rNodeScene=ProjectContext::getInstance().getNodeScene();
	if (rNodeScene) {
		getMainWindow().getSceneTreeDock().setSceneEditable(true);
		getMainWindow().getSceneTreeDock().addSceneNode(nullptr, rNodeScene);
	}
	switchProperties(true, nullptr);
}

void GuiContext::resourceSwitched() {
	getMainWindow().getSceneTreeDock().clearResource();
	NodeResource* rNodeResource=ProjectContext::getInstance().getCurrentResource();
	if (rNodeResource) {
		getMainWindow().getSceneTreeDock().setResourceEditable(true);
		getMainWindow().getSceneTreeDock().addResourceNode(nullptr, rNodeResource);
	}
	//switchProperties(true, nullptr);
}

void GuiContext::switchProperties(bool isSceneProperty, Node* rNode) {
	if (isSceneProperty) {
		getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	}
}

