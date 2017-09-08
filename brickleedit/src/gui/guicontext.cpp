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
		if (getCurrentScene()) {
			rPartTitle+=" ["+getCurrentScene()->getName()+"]";
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
		setCurrentScene("");
		setCurrentResource("");
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
		getMainWindow().getNewSceneDialog().setName("New Scene");
		getMainWindow().getNewSceneDialog().show();
	}
}


void GuiContext::onNewResourceClicked() {
	if (!ProjectContext::getInstance().getNodeProject()) {
		QMessageBox::warning(&getMainWindow(), tr("Error"), tr("Please open existing project or create a new project first"), QMessageBox::Ok);
	} else {
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
			createNewScene(rStartupSceneName, true);
        }
        {
			string rProjectResourceName=DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getResourcesSubPath(),ProjectContext::getInstance().getNodeProject()->getProjectResource());
			createNewResource(rProjectResourceName, true);
        }
    }
	return rv;
}

bool GuiContext::loadProject(const string&rProjectPathWithFileAbs) {
	QFileInfo fi(QString::fromStdString(rProjectPathWithFileAbs));
	QString path=fi.absolutePath();
	string rProjectPathAbs=path.toStdString();
	string rProjectName=fi.fileName().toStdString();

	bool rv=ProjectContext::getInstance().loadProject(rProjectPathAbs, rProjectName);
	if (rv) {
		vector<Node*> v=ProjectContext::getInstance().getNodeProject()->getChildNodesWithNodeType(NodeType::SceneInfo);
		for(auto rNode :  v) {
			if (rNode->getNodeType()==NodeType::SceneInfo) {
				NodeSceneInfo* rNodeSceneInfo=static_cast<NodeSceneInfo*>(rNode);
				if (rNodeSceneInfo->getIsDefault()) {
					NodeScene*rNodeScene=getOrLoadSceneByName(rNodeSceneInfo->getPath());
					if (rNodeScene) {
						setCurrentScene(rNodeScene->getName());
					}
					break;
				}
			}
		}
		v=ProjectContext::getInstance().getNodeProject()->getChildNodesWithNodeType(NodeType::ResourceInfo);
		for(auto rNode :  v) {
			if (rNode->getNodeType()==NodeType::ResourceInfo) {
				NodeResourceInfo* rNodeResourceInfo=static_cast<NodeResourceInfo*>(rNode);
				if (rNodeResourceInfo->getIsDefault()) {
					NodeResource*rNodeResource=getOrLoadResourceByName(rNodeResourceInfo->getPath());
					if (rNodeResource) {
						setCurrentResource(rNodeResource->getName());
					}
					break;
				}
			}
		}
		setWindowTitle();
		projectSwitched();
	}
	return rv;
}

NodeSceneInfo* GuiContext::getSceneInfoByName(const string& rSceneName) {
	NodeSceneInfo *rNodeSceneInfo=nullptr;
	Node *rNode=ProjectContext::getInstance().getNodeProject()->getChildNodeWithNameAndNodeType(rSceneName, NodeType::SceneInfo);
	if (rNode && rNode->getNodeType()==NodeType::SceneInfo) {
		rNodeSceneInfo=static_cast<NodeSceneInfo*>(rNode);
	}
	return rNodeSceneInfo;
}

NodeScene* GuiContext::getOrLoadSceneByName(const string& rSceneName) {
	NodeScene *rNodeScene=nullptr;
	if (getSceneInfoByName(rSceneName)) {
		rNodeScene=ProjectContext::getInstance().getNodeSceneByName(rSceneName);
		if (!rNodeScene) {
			if (ProjectContext::getInstance().load(NodeInfoType::Scene, rSceneName)) {
				rNodeScene=ProjectContext::getInstance().getNodeSceneByName(rSceneName);
			}
		}
	}
	return rNodeScene;
}

bool GuiContext::createNewScene(const string& rSceneName, bool asDefault) {
	bool rv=false;
	if (getSceneInfoByName(rSceneName)) {
		rv=true;
	} else {
		rv=ProjectContext::getInstance().createNew(NodeInfoType::Scene, rSceneName);
		if (rv) {
			NodeSceneInfo *rNodeSceneInfo=static_cast<NodeSceneInfo*>(getInstanceFromNodeType(NodeType::SceneInfo, true));
			if (rNodeSceneInfo) {
				rNodeSceneInfo->setName(rSceneName);
				rNodeSceneInfo->setPath(rSceneName);
				rNodeSceneInfo->setIsDefault(asDefault);
				ProjectContext::getInstance().getNodeProject()->addChildNode(rNodeSceneInfo);
			}
			if (ProjectContext::getInstance().getNodeSceneByName(rSceneName)) {
				QDir tmp(QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), rSceneName))));
				tmp.mkpath(".");
			}

			setCurrentScene(rSceneName);
			setWindowTitle();
			sceneSwitched();
		}
	}
	if (rv) {
	}
	return rv;
}

void GuiContext::setCurrentScene(const string& rSceneName) {
	mCurrentScene=nullptr;
	if (rSceneName.size()>0) {
		NodeScene *rNodeScene=ProjectContext::getInstance().getNodeSceneByName(rSceneName);
		if (rNodeScene) {
			mCurrentScene=rNodeScene;
		}
	}
}

NodeScene* GuiContext::getCurrentScene() {
	return mCurrentScene;
}


NodeResourceInfo* GuiContext::getResourceInfoByName(const string& rResourceName) {
	NodeResourceInfo *rNodeResourceInfo=nullptr;
	Node *rNode=ProjectContext::getInstance().getNodeProject()->getChildNodeWithNameAndNodeType(rResourceName, NodeType::ResourceInfo);
	if (rNode && rNode->getNodeType()==NodeType::ResourceInfo) {
		rNodeResourceInfo=static_cast<NodeResourceInfo*>(rNode);
	}
	return rNodeResourceInfo;
}

NodeResource* GuiContext::getOrLoadResourceByName(const string& rResourceName) {
	NodeResource *rNodeResource=nullptr;
	if (getResourceInfoByName(rResourceName)) {
		rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rResourceName);
		if (!rNodeResource) {
			if (ProjectContext::getInstance().load(NodeInfoType::Resource, rResourceName)) {
				rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rResourceName);
			}
		}
	}
	return rNodeResource;
}

bool GuiContext::createNewResource(const string& rName, bool asDefault) {
	bool rv=false;
	if (getResourceInfoByName(rName)) {
		rv=true;
	} else {
		NodeResource *rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
		if (rNodeResource==nullptr) {
			rv=ProjectContext::getInstance().createNew(NodeInfoType::Resource, rName);
			if (rv) {
				NodeResourceInfo *rNodeResourceInfo=static_cast<NodeResourceInfo*>(getInstanceFromNodeType(NodeType::ResourceInfo, true));
				if (rNodeResourceInfo) {
					rNodeResourceInfo->setName(rName);
					rNodeResourceInfo->setPath(rName);
					rNodeResourceInfo->setIsDefault(asDefault);

					ProjectContext::getInstance().getNodeProject()->addChildNode(rNodeResourceInfo);
				}
				if (ProjectContext::getInstance().getNodeResourceByName(rName)) {
					QDir tmp(QString::fromStdString(DirUtil::trimFileName(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), rName))));
					tmp.mkpath(".");
				}

				setCurrentResource(rName);
				resourceSwitched();
			}
		}
	}
	return rv;
}

void GuiContext::setCurrentResource(const string& rResourceName) {
	mCurrentResource=nullptr;
	if (rResourceName.size()>0) {
		NodeResource *rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rResourceName);
		if (rNodeResource) {
			mCurrentResource=rNodeResource;
		}
	}
}

NodeResource* GuiContext::getCurrentResource() {
	return mCurrentResource;
}

void GuiContext::onCreateNewNode(QString rNodeTypeName, NodeInfoType rNodeInfoType) {
	QTreeWidgetItem *parent=nullptr;
	if (rNodeInfoType==NodeInfoType::Resource) {
		parent=getMainWindow().getSceneTreeDock().getRootItem(rNodeInfoType);
	} else {
		parent=getMainWindow().getSceneTreeDock().getSelectedItem(rNodeInfoType);
	}
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

void GuiContext::updateChildNodes(Node *rNode_parent, NodeInfoType rNodeInfoType) {
	getMainWindow().getSceneTreeDock().updateChildNodes(rNode_parent, rNodeInfoType);
	updateGlWidget();
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
	NodeScene* rNodeScene=getCurrentScene();
	if (rNodeScene) {
		getMainWindow().getSceneTreeDock().setSceneEditable(true);
		getMainWindow().getSceneTreeDock().addNode(nullptr, rNodeScene, NodeInfoType::Scene);
	}
	switchProperties(nullptr, NodeInfoType::Scene);
}

void GuiContext::resourceSwitched() {
	getMainWindow().getSceneTreeDock().clearResource();
	NodeResource* rNodeResource=getCurrentResource();
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
