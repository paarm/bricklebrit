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
		ProjectContext::getInstance().closeProject(true);
		projectSwitched();
		setWindowTitle();
	}
}

void GuiContext::onSaveProjectClicked() {
	if (ProjectContext::getInstance().getNodeProject()) {
		//QDir tmp(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()));
		//tmp.mkpath(".");
		//if (ProjectContext::getInstance().getNodeScene()) {
		//   QDir tmp(QString::fromStdString(ProjectContext::getInstance().getScenePathAbs()));
		//  tmp.mkpath(".");
		//}
        ProjectContext::getInstance().save();
	}
}

void GuiContext::onNewSceneClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString path=getVirtualProjectPath();
		if (ProjectContext::getInstance().isSceneAvailable()) {
			path=QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(getVirtualProjectPath().toStdString(), ProjectContext::getInstance().getNodeScene()->getName())));
		}
		getMainWindow().getNewSceneDialog().setScenePath(path);
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
			path=QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(getVirtualProjectPath().toStdString(), ProjectContext::getInstance().getNodeScene()->getName())));
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
				string rName=DirUtil::subPart1FromPart2(ProjectContext::getInstance().getProjectPathAbs(), file.toStdString());
				if (!loadCurrentScene(rName)) {
					QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Scene not found"), QMessageBox::Ok);
				}
			}
		}
	}
}

bool GuiContext::loadCurrentScene(const string&rName) {

	bool rv=ProjectContext::getInstance().load(NodeInfoType::Scene, ProjectContext::getInstance().getProjectPathAbs(), rName);
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
		if (ProjectContext::getInstance().getCurrentResource()) {
			path=QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(getVirtualProjectPath().toStdString(), ProjectContext::getInstance().getCurrentResource()->getName())));
		}
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
				string rName=DirUtil::subPart1FromPart2(ProjectContext::getInstance().getProjectPathAbs(), file.toStdString());
				if (!loadCurrentResource(rName)) {
					QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Resource not found"), QMessageBox::Ok);
				}
			}
		}
	}
}

bool GuiContext::loadCurrentResource(const string&rName) {
	bool rv=ProjectContext::getInstance().load(NodeInfoType::Resource, ProjectContext::getInstance().getProjectPathAbs(), rName);
	NodeResource *rNodeResource=nullptr;
	if (rv) {
		rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
	}
	ProjectContext::getInstance().setCurrentResource(rNodeResource);
	resourceSwitched();
	return rv;
}

void GuiContext::onNewResourceClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
		QString path=getVirtualProjectPath();
		if (ProjectContext::getInstance().getCurrentResource()) {
			path=QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(getVirtualProjectPath().toStdString(), ProjectContext::getInstance().getCurrentResource()->getName())));
		}
		//if (ProjectContext::getInstance().isSceneAvailable()) {
		//	getMainWindow().getNewSceneDialog().setScenePath(toVirtualPath(QString::fromStdString(ProjectContext::getInstance().getScenePathAbs())));
		//} else {
		//}
		getMainWindow().getNewResourceDialog().setScenePath(path);
		getMainWindow().getNewResourceDialog().setName("New Resource");
		getMainWindow().getNewResourceDialog().show();
	}
}


bool GuiContext::createNewProject(const string& rProjectPathAbs, const string& rProjectName) {
	bool rv=ProjectContext::getInstance().createNewProject(rProjectPathAbs, rProjectName);
	if (rv) {
        ProjectContext::getInstance().getNodeProject()->setScenesSubPath("scenes");
		ProjectContext::getInstance().getNodeProject()->setStartScene("StartupScene.brscn");
        ProjectContext::getInstance().getNodeProject()->setResourcesSubPath("resources");
        ProjectContext::getInstance().getNodeProject()->setProjectResource("ProjectResource.brres");


		setWindowTitle();
		projectSwitched();

        QDir tmp(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()));
        tmp.mkpath(".");
        {
			string rStartupSceneName=DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getScenesSubPath(),ProjectContext::getInstance().getNodeProject()->getStartScene());
			createNewScene(rStartupSceneName);

            if (ProjectContext::getInstance().getNodeScene()) {
				QDir tmp(QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), ProjectContext::getInstance().getNodeScene()->getName()))));
                tmp.mkpath(".");
            }
        }
        {
			string rProjectResourceName=DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getResourcesSubPath(),ProjectContext::getInstance().getNodeProject()->getProjectResource());
			createNewResource(rProjectResourceName);
            if (ProjectContext::getInstance().getCurrentResource()) {
				QDir tmp(QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), ProjectContext::getInstance().getCurrentResource()->getName()))));
                tmp.mkpath(".");
            }
        }
    }
	return rv;
}

bool GuiContext::loadProject(const string&rProjectPathWithFileAbs) {
	QFileInfo fi(QString::fromStdString(rProjectPathWithFileAbs));
	QString path=fi.absolutePath();
	string rProjectPathAbs=path.toStdString();
	string rProjectName=fi.fileName().toStdString();

	bool rv=ProjectContext::getInstance().load(NodeInfoType::Project, rProjectPathAbs, rProjectName);
	if (rv) {
		loadCurrentScene(DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getScenesSubPath(), ProjectContext::getInstance().getNodeProject()->getStartScene()));
		loadCurrentResource(DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getResourcesSubPath(), ProjectContext::getInstance().getNodeProject()->getProjectResource()));

		setWindowTitle();
		projectSwitched();
	}
	return rv;
}

bool GuiContext::createNewScene(const string& rSceneName) {
	bool rv=ProjectContext::getInstance().createNew(NodeInfoType::Scene, rSceneName);
	if (rv) {
		setWindowTitle();
		sceneSwitched();
	}
	return rv;
}

bool GuiContext::createNewResource(const string& rName) {
	bool rv=true;
	NodeResource *rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
	if (rNodeResource==nullptr) {
		rv=ProjectContext::getInstance().createNew(NodeInfoType::Resource, rName);
		if (rv) {
			rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
		}
	}
	ProjectContext::getInstance().setCurrentResource(rNodeResource);
	resourceSwitched();

	return rv;
}

void GuiContext::onCreateNewNode(QString rNodeTypeName, NodeInfoType rNodeInfoType) {
	QTreeWidgetItem *parent=getMainWindow().getSceneTreeDock().getSelectedItem(rNodeInfoType);
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
		getMainWindow().getSceneTreeDock().addNode(parent, rNode, rNodeInfoType);
	}
}

void GuiContext::projectSwitched() {
	getMainWindow().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	getMainWindow().setProjectRequireSave(ProjectContext::getInstance().getNodeProject()); // TODO own mechanism
	getMainWindow().getSceneTreeDock().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	sceneSwitched();
	resourceSwitched();
}

void GuiContext::sceneSwitched() {
	getMainWindow().getSceneTreeDock().clearScene();
	NodeScene* rNodeScene=ProjectContext::getInstance().getNodeScene();
	if (rNodeScene) {
		getMainWindow().getSceneTreeDock().setSceneEditable(true);
		getMainWindow().getSceneTreeDock().addNode(nullptr, rNodeScene, NodeInfoType::Scene);
	}
	switchProperties(nullptr, NodeInfoType::Scene);
}

void GuiContext::resourceSwitched() {
	getMainWindow().getSceneTreeDock().clearResource();
	NodeResource* rNodeResource=ProjectContext::getInstance().getCurrentResource();
	if (rNodeResource) {
		getMainWindow().getSceneTreeDock().setResourceEditable(true);
		getMainWindow().getSceneTreeDock().addNode(nullptr, rNodeResource, NodeInfoType::Resource);
	}
	switchProperties(nullptr, NodeInfoType::Resource);
}

void GuiContext::switchProperties(Node* rNode, NodeInfoType rNodeInfoType) {
	getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode, rNodeInfoType);
	if (rNodeInfoType==NodeInfoType::Scene) {
		replaceSelectedSceneNode(rNode);
		updateGlWidget();
	}
}

void GuiContext::currentPropertyValueChanged(Node* rNode, NodeInfoType rNodeInfoType) {
	getMainWindow().getSceneTreeDock().updateNodeName(rNode, rNodeInfoType);
	updateGlWidget();
}

void GuiContext::updateGlWidget() {
	getMainWindow().getSceneGlWidget().update();
}

void GuiContext::clearSelectedSceneNodes() {
	mSelectedSceneNodes.clear();
}

void GuiContext::clearSelectedSceneNode(Node *rDeselectedNode) {
	if (rDeselectedNode!=nullptr) {
		const auto &it=std::find_if(mSelectedSceneNodes.begin(), mSelectedSceneNodes.end(), [rDeselectedNode] (Node* entry) {
			return rDeselectedNode==entry;
		}
		);
		if (it!=mSelectedSceneNodes.end()) {
			mSelectedSceneNodes.erase(it);
		}
	}
}

bool GuiContext::isSceneNodeSelected(Node* rTestNode) {
	bool rv=false;
	if (rTestNode!=nullptr) {
		for(auto rNode : mSelectedSceneNodes) {
			if (rNode==rTestNode) {
				rv=true;
				break;
			}
		}
	}
	return rv;
}

void GuiContext::replaceSelectedSceneNode(Node *rSelectedNode) {
	clearSelectedSceneNodes();
	setSceneNodeAsSelected(rSelectedNode);
}

void GuiContext::setSceneNodeAsSelected(Node* rSelectedNode) {
	if (!isSceneNodeSelected(rSelectedNode)) {
		if (rSelectedNode!=nullptr) {
			mSelectedSceneNodes.push_back(rSelectedNode);
		}
	}
}
