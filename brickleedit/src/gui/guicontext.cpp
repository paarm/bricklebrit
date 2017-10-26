#include "guicontext.h"
#include <QMessageBox>
#include "treeutil.h"

#include "grideditor/grideditor.h"
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
		mCurrentScene=nullptr;
		mCurrentResource=nullptr;
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
		ProjectContext::getInstance().getNodeProject()->setGridSize(PointInt(32,32));
		ProjectContext::getInstance().getNodeProject()->setGridActive(true);

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
	projectSwitched();
	if (rv) {
		NodeScene* rNodeScene=ProjectContext::getInstance().getDefaultScene();
		if (rNodeScene) {
			setCurrentScene(rNodeScene->getName());
		}
		NodeResource* rNodeResource=ProjectContext::getInstance().getDefaultResource();
		if (rNodeResource) {
			setCurrentResource(rNodeResource->getName());
		}
		getMainWindow().getSceneTreeDock().updateSceneDropdownWithCurrent();
		getMainWindow().getSceneTreeDock().updateResourceDropdownWithCurrent();
		setWindowTitle();
		//projectSwitched();
	}
	return rv;
}



bool GuiContext::createNewScene(const string& rSceneName, bool asDefault) {
	bool rv=false;
	if (ProjectContext::getInstance().getSceneInfoByName(rSceneName)) {
		rv=true;
	} else {
		rv=ProjectContext::getInstance().createNewScene(rSceneName);
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
			NodeLayer* rNodeLayer=static_cast<NodeLayer*>(getInstanceFromNodeType(NodeType::Layer, true));
			rNodeLayer->setName("Layer 1");
			NodeScene *rNodeScene=ProjectContext::getInstance().getOrLoadSceneByName(rSceneName);
			if (rNodeScene) {
				rNodeScene->addChildNode(rNodeLayer);
			}

			setCurrentScene(rSceneName);
			getMainWindow().getSceneTreeDock().updateSceneDropdownWithCurrent();

		}
	}
	if (rv) {
	}
	return rv;
}


void GuiContext::setCurrentScene(const string& rName) {
	if (getCurrentScene()==nullptr || getCurrentScene()->getName()!=rName) {
		mSelectionManager.deselectAllNodes();
		if (rName.size()>0) {
			NodeScene *rNodeScene=ProjectContext::getInstance().getOrLoadSceneByName(rName);
			mCurrentScene=rNodeScene;
		} else {
			mCurrentScene=nullptr;
		}
		mLayerManager.currentSceneChanged();

		getMainWindow().setSceneAvailable(getCurrentScene());
		//getMainWindow().getSceneTreeDock().switchToScene(getCurrentScene());
		getMainWindow().getBrushDock().setBrushEnabled(getCurrentScene()?true:false);

		//setCurrentPaintCanvas(getCurrentScene(), false);
		setWindowTitle();
		updateGlWidget();
	}
}

NodeScene* GuiContext::getCurrentScene() {
	return mCurrentScene;
}


bool GuiContext::createNewResource(const string& rName, bool asDefault) {
	bool rv=false;
	if (ProjectContext::getInstance().getResourceInfoByName(rName)) {
		rv=true;
	} else {
		NodeResource *rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
		if (rNodeResource==nullptr) {
			rv=ProjectContext::getInstance().createNewResource(rName);
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
				getMainWindow().getSceneTreeDock().updateResourceDropdownWithCurrent();
			}
		}
	}
	return rv;
}

void GuiContext::setCurrentResource(const string& rName) {
	if (getCurrentResource()==nullptr || getCurrentResource()->getName()!=rName) {
		if (rName.size()>0) {
			NodeResource *rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rName);
			mCurrentResource=rNodeResource;
		} else {
			mCurrentResource=nullptr;
		}
		getMainWindow().getSceneTreeDock().switchToResource(getCurrentResource());
		setWindowTitle();
		updateGlWidget();
	}
}

NodeResource* GuiContext::getCurrentResource() {
	return mCurrentResource;
}

#if 0
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
			Node* parentNode=TreeUtil::getNodeFromTreeItem(parent);
			if (parentNode) {
				parentNode->addChildNode(rNode);
			}
		}
		getMainWindow().getSceneTreeDock().addNode(parent, rNode, rNodeInfoType);
	}
}
#endif

void GuiContext::updateChildNodes(Node *rNode_parent) {
	getMainWindow().getSceneTreeDock().updateChildNodes(rNode_parent);
	updateGlWidget();
}

void GuiContext::insertNewSceneNode(Node *rNode) {
	if (rNode) {
		//Node *rParentNode=getMainWindow().getSceneTreeDock().getSelectedSceneNode();
		Node *rParentNode=getCurrentPaintCanvas();
		if (rParentNode) {
			rParentNode->addChildNode(rNode);
			//getMainWindow().getSceneTreeDock().addSceneNodeToParent(rNode, rParentNode);
			updateNodeName(rNode);
			updateGlWidget();
		}
	}
}

void GuiContext::eraseNodeFromSceneWithId(int rNodeId) {
    NodeScene *rNodeScene=getCurrentScene();
    if (rNodeScene) {
        Node *rNodeToDelete=rNodeScene->getChildNodeWithNodeIdRecursive(rNodeId);
        if (rNodeToDelete && rNodeToDelete->getParent()) {
			//getMainWindow().getSceneTreeDock().eraseSceneNode(rNodeToDelete);
			if (getCurrentPaintCanvas() && getCurrentPaintCanvas()->isThisNodeOrParentOrGrandParentOf(rNodeToDelete)) {
				setCurrentPaintCanvas(rNodeScene, false);
            }
			getSelectionManager().deselectAllIfChildOf(rNodeToDelete);

            rNodeToDelete->getParent()->deleteChildNode(rNodeToDelete);
        }
    }
}

void GuiContext::insertNewResourceNode(Node *rNode) {
	if (rNode) {
		Node *rParentNode=getMainWindow().getSceneTreeDock().getRootResourceNode();
		if (rParentNode) {
			rParentNode->addChildNode(rNode);
			getMainWindow().getSceneTreeDock().addResourceNodeToRootItem(rNode);
			updateGlWidget();
		}
	}
}


void GuiContext::updateNodeName(Node *rNode) {
	getMainWindow().getSceneTreeDock().updateNodeName(rNode);
	getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	//updateGlWidget();
}

void GuiContext::projectSwitched() {
	getMainWindow().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	getMainWindow().setProjectRequireSave(ProjectContext::getInstance().getNodeProject()); // TODO own mechanism
	getMainWindow().getSceneTreeDock().setProjectAvailable(ProjectContext::getInstance().getNodeProject());
	//sceneSwitched();
	//resourceSwitched();
}

#if 0
void GuiContext::switchProperties(Node* rNode, NodeInfoType rNodeInfoType) {
	getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	if (rNodeInfoType==NodeInfoType::Scene) {
		replaceSelectedSceneNode(rNode);
	}
	updateGlWidget();
}
#endif
void GuiContext::currentPropertyValueChanged(Node* rNode) {
	getMainWindow().getSceneTreeDock().updateNodeName(rNode);
	updateGlWidget();
}

bool GuiContext::isNodeVisibleOn(Node *rNode) {
	bool rv=false;
	if (rNode) {
		if (rNode->getNodeType()==NodeType::Layer) {
			NodeLayer* rNodeLayer=static_cast<NodeLayer*>(rNode);
			rv=rNodeLayer->getVisible();
		} else {
			rv=true;
		}
	}
	return rv;
}

bool GuiContext::isNodeLocked(Node *rNode) {
	bool rv=false;
	if (rNode) {
		if (rNode->getNodeType()==NodeType::Layer) {
			NodeLayer* rNodeLayer=static_cast<NodeLayer*>(rNode);
			rv=rNodeLayer->getLocked();
		}
	}
	return rv;
}

void GuiContext::updateGlWidget() {
	getMainWindow().getSceneGlWidget().update();
}

void GuiContext::setSceneNodeInTreeAsSelected(Node *rNode) {
	getMainWindow().getSceneTreeDock().setSceneNodeAsSelected(rNode);
}

SelectionManager& GuiContext::getSelectionManager() {
	return mSelectionManager;
}

LayerManager& GuiContext::getLayerManager() {
	return mLayerManager;
}

void GuiContext::onZoomLevelChanged(int rZoomLevel) {
	getMainWindow().getToolBar().updateZoomLevel(rZoomLevel);
}

void GuiContext::onToolSelectionActivated() {
	setCurrentTool(Tool::Selection);
}

void GuiContext::onToolBrushActivated() {
	setCurrentTool(Tool::Brush);
}

void GuiContext::onGridCheckedChanged() {
	if (ProjectContext::getInstance().getNodeProject()) {
		ProjectContext::getInstance().getNodeProject()->setGridActive(GuiContext::getMainWindow().getActionGrid()->isChecked());
	}
}

void GuiContext::onGridEdit() {
	PointInt gridSize;
	PointInt gridOffset;
	if (ProjectContext::getInstance().getNodeProject()) {
		gridSize=ProjectContext::getInstance().getNodeProject()->getGridSize();
		gridOffset=ProjectContext::getInstance().getNodeProject()->getGridOffset();
	}
	GridEditor *rGridEditor = new GridEditor(gridSize.x, gridSize.y, gridOffset.x, gridOffset.y, &GuiContext::getMainWindow());
	rGridEditor->setModal(true);
	rGridEditor->show();
}

void GuiContext::updateGridSettings(int gridX, int gridY, int offsetX, int offsetY) {
	if (ProjectContext::getInstance().getNodeProject()) {
		ProjectContext::getInstance().getNodeProject()->setGridSize(PointInt(gridX, gridY));
		ProjectContext::getInstance().getNodeProject()->setGridOffset(PointInt(offsetX, offsetY));
	}
}

bool GuiContext::isGridActive() {
	bool rv=false;
	if (ProjectContext::getInstance().getNodeProject()) {
		rv=ProjectContext::getInstance().getNodeProject()->getGridActive();
	}
	return rv;
}

void GuiContext::setCurrentTool(Tool rTool) {
	mCurrentTool=rTool;
}

Tool GuiContext::getCurrentTool() {
	return mCurrentTool;
}

void GuiContext::setCurrentPaintCanvas(Node2d *rNode, bool switchToBrushTool) {
	mCurrentPaintCanvas=rNode;
    if (rNode && switchToBrushTool && getCurrentTool()!=Tool::Brush) {
        setCurrentTool(Tool::Brush);
        GuiContext::getInstance().getMainWindow().getActionToolBrush()->setChecked(true);
    }
}

void GuiContext::onPickAsBrush() {
	Node *rNode=mSelectionManager.getLatestSelected();
	if (rNode && rNode->getNodeType()==NodeType::Sprite) {
		NodeSprite * rNodeSprite=static_cast<NodeSprite*>(rNode);
		Node * rNodeTextureOrAnimation=getFrameReferenceNodeForSprite(rNodeSprite);
		if (rNodeTextureOrAnimation) {
			SelectedItem rSelectedItem=prepareSelectedNodeFromTextureOrAnimationNode(rNodeTextureOrAnimation, rNodeSprite->getFrameRef().resourcefile);
			SelectedItemPref rSelectedItemPref;
			rSelectedItemPref.sizeWH=rNodeSprite->getSize();
			rSelectedItemPref.scale=rNodeSprite->getScale();
			rSelectedItemPref.rotation=rNodeSprite->getRotation();
			rSelectedItemPref.flipX=rNodeSprite->getFlipX();
			rSelectedItemPref.flipY=rNodeSprite->getFlipY();

			setCurrentBrush(rSelectedItem, &rSelectedItemPref);
		}
	}
}

void GuiContext::onEraseSelected() {
    const vector<Node*> v=mSelectionManager.getSelectedNodes();
    if (v.size()>0) {
        vector<int> vId;
        for (Node* n : v) {
            vId.push_back(n->getId());
        }
        for (int rNodeId : vId) {
			eraseNodeFromSceneWithId(rNodeId);
        }
        mSelectionManager.deselectAllNodes();
        updateGlWidget();
    }
}

void GuiContext::setCurrentBrush(SelectedItem rSelectedItem, SelectedItemPref *rSelectedItemPref) {
    getMainWindow().getBrushDock().setAsBrush(rSelectedItem, rSelectedItemPref);
    if (getCurrentTool()!=Tool::Brush) {
        setCurrentTool(Tool::Brush);
        GuiContext::getInstance().getMainWindow().getActionToolBrush()->setChecked(true);
    }
}

Node2d* GuiContext::getCurrentPaintCanvas() {
	return mCurrentPaintCanvas;
}

Node * GuiContext::getFrameReferenceNodeForSprite(NodeSprite *rNodeSprite) {
    Node *rv=nullptr;
    if (rNodeSprite) {
        NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rNodeSprite->getFrameRef().resourcefile);
        if (rNodeResource) {
            Node *rNodeTextureOrAnimation=rNodeResource->getNodeWithNodeId(rNodeSprite->getFrameRef().textureid);
            if (rNodeTextureOrAnimation) {
                //Node *rNodeFrame=nullptr;
                if (rNodeSprite->getFrameRef().frame.length()>0) {
                    rv=rNodeTextureOrAnimation->getChildNodeWithName(rNodeSprite->getFrameRef().frame);
                }
                if (!rv) {
                    rv=rNodeTextureOrAnimation;
                }
            }
        }
    }
    return rv;
}

SelectedItem GuiContext::prepareSelectedNodeFromTextureOrAnimationNode(Node* rNode, const string& rResourceTextureOrAnimation) {
    SelectedItem rSelectedItem;
    rSelectedItem.resourceName=rResourceTextureOrAnimation;

    if (rNode) {
        if (rNode->getNodeType()==NodeType::Texture) {
            rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNode);
        } else if (rNode->getNodeType()==NodeType::TextureFrame) {
            rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNode->getParent());
            rSelectedItem.rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
        } else if (rNode->getNodeType()==NodeType::Animation) {
            rSelectedItem.rNodeAnimation=static_cast<NodeAnimation*>(rNode);
            if (rSelectedItem.rNodeAnimation->getChildCount()>0) {
                rSelectedItem.rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rSelectedItem.rNodeAnimation->getNodeFromIndex(0));
            }
        } else if (rNode->getNodeType()==NodeType::AnimationFrame) {
            rSelectedItem.rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNode);
            rSelectedItem.rNodeAnimation=static_cast<NodeAnimation*>(rNode->getParent());
        }
        if (rSelectedItem.rNodeAnimationFrame) {
            NodeResource *rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile);
            if (rNodeResource) {
                rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNodeResource->getNodeWithNodeId(rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid));
                if (rSelectedItem.rNodeTexture) {
                    if (!rSelectedItem.rNodeAnimationFrame->getFrameRef().frame.empty()) {
                        rSelectedItem.rNodeTextureFrame=static_cast<NodeTextureFrame*>(rSelectedItem.rNodeTexture->getChildNodeWithNameAndNodeType(rSelectedItem.rNodeAnimationFrame->getFrameRef().frame, NodeType::TextureFrame));
                    }
                }
            }
        }
    }
    return rSelectedItem;
}


