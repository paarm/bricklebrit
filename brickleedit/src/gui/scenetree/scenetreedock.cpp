#include "scenetreedock.h"
#include "ui_scenetreedock.h"
#include "../guicontext.h"
#include "../textureframeeditor/textureframeeditor.h"
#include "../animationframeeditor/animationframeeditor.h"
#include "../treeutil.h"

SceneTreeDock::SceneTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SceneTreeDock)
{
	ui->setupUi(this);
	ui->sceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());

	ui->resourceMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewResource());

	setSceneEditable(false);
	setResourceEditable(false);
	setProjectAvailable(false);
}

SceneTreeDock::~SceneTreeDock()
{
	delete ui;
}

void SceneTreeDock::clearScene() {
	ui->treeWidget->clear();
	setSceneEditable(false);
}

void SceneTreeDock::clearResource() {
	ui->treeWidgetResources->clear();
	setResourceEditable(false);
}

void SceneTreeDock::setSceneEditable(bool isEditable) {
	ui->newNode->setEnabled(isEditable);
}

void SceneTreeDock::setResourceEditable(bool isEditable) {
	ui->newResource->setEnabled(isEditable);
}

void SceneTreeDock::setProjectAvailable(bool isActive) {
	ui->sceneMenu->setEnabled(isActive);
	ui->resourceMenu->setEnabled(isActive);
}

void SceneTreeDock::on_newNode_clicked()
{
	if (newSceneNodeDialog==nullptr) {
		newSceneNodeDialog=new NewSceneNodeDialog(NodeInfoType::Scene, this);
		QObject::connect(newSceneNodeDialog, &NewSceneNodeDialog::newNodeSelected, &GuiContext::getInstance(), &GuiContext::onCreateNewNode);
	}
	newSceneNodeDialog->show();
}

void SceneTreeDock::on_newResource_clicked()
{
	if (newResourceNodeDialog==nullptr) {
		newResourceNodeDialog=new NewSceneNodeDialog(NodeInfoType::Resource, this);
		QObject::connect(newResourceNodeDialog, &NewSceneNodeDialog::newNodeSelected, &GuiContext::getInstance(), &GuiContext::onCreateNewNode);
	}
	newResourceNodeDialog->show();
}

int SceneTreeDock::getActiveTreeTabIndex() {
	return ui->tabWidget->currentIndex();
}

QTreeWidgetItem* SceneTreeDock::getSelectedItem(NodeInfoType rNodeInfoType) {
	QTreeWidgetItem* rv=nullptr;
	if (rNodeInfoType==NodeInfoType::Scene) {
		QList<QTreeWidgetItem*>selectedItems=ui->treeWidget->selectedItems();
		if (!selectedItems.isEmpty()) {
			rv=selectedItems[0];
		}
	} else if (rNodeInfoType==NodeInfoType::Resource) {
		QList<QTreeWidgetItem*>selectedItems=ui->treeWidgetResources->selectedItems();
		if (!selectedItems.isEmpty()) {
			rv=selectedItems[0];
		}
	}
	return rv;
}

QTreeWidgetItem* SceneTreeDock::getRootItem(NodeInfoType rNodeInfoType) {
	QTreeWidgetItem* rv=nullptr;
	if (rNodeInfoType==NodeInfoType::Scene) {
		if (ui->treeWidget->topLevelItemCount()>0) {
			rv=ui->treeWidget->topLevelItem(0);
		}
	} else if (rNodeInfoType==NodeInfoType::Resource) {
		if (ui->treeWidgetResources->topLevelItemCount()>0) {
			rv=ui->treeWidgetResources->topLevelItem(0);
		}
	}
	return rv;
}


Node* SceneTreeDock::getSelectedNode(NodeInfoType rNodeInfoType) {
	QTreeWidgetItem*r=getSelectedItem(rNodeInfoType);
	Node *rv=nullptr;
	if (r) {
		rv=TreeUtil::getNodeFromTreeItem(r);
	}
	return rv;
}

void SceneTreeDock::deleteChilds(QTreeWidgetItem *parent) {
	if (parent) {
		int cnt=parent->childCount();
		for (int i=cnt-1;i>=0;i--) {
			QTreeWidgetItem *child=parent->child(i);
			if (child) {
				parent->removeChild(child);
				delete child;
			}
		}
	}
}

QTreeWidgetItem* SceneTreeDock::searchTreeWidgetItemByNode(QTreeWidgetItem *parent, Node* rNode) {
	QTreeWidgetItem* rv=nullptr;

	if (parent && TreeUtil::getNodeFromTreeItem(parent)==rNode) {
		rv=parent;
	}
	if (!rv) {
		int count=parent->childCount();
		for (int i=0;i<count;i++) {
			QTreeWidgetItem *r=parent->child(i);
			if (r && TreeUtil::getNodeFromTreeItem(r)==rNode) {
				rv=r;
			}
		}
	}
	if (!rv) {
		int count=parent->childCount();
		for (int i=0;i<count && !rv;i++) {
			QTreeWidgetItem *r=parent->child(i);
			rv=searchTreeWidgetItemByNode(r, rNode);
		}
	}
	return rv;
}

QTreeWidget* SceneTreeDock::getTreeWidgetFromNodeInfoType(NodeInfoType rNodeInfoType) {
	QTreeWidget* rv=nullptr;
	if (rNodeInfoType==NodeInfoType::Scene) {
		rv=ui->treeWidget;
	} else {
		rv=ui->treeWidgetResources;
	}
	return rv;
}

void SceneTreeDock::updateChildNodes(Node* rNode, NodeInfoType rNodeInfoType) {
	QTreeWidget* rQTreeWidget=getTreeWidgetFromNodeInfoType(rNodeInfoType);
	if (rQTreeWidget) {
		int count=rQTreeWidget->topLevelItemCount();
		QTreeWidgetItem* rQTreeWidgetItem_found=nullptr;
		for (int i=0;i<count && !rQTreeWidgetItem_found;i++) {
			QTreeWidgetItem* p=rQTreeWidget->topLevelItem(i);

			rQTreeWidgetItem_found=searchTreeWidgetItemByNode(p,rNode);
		}
		if (rQTreeWidgetItem_found) {
			deleteChilds(rQTreeWidgetItem_found);

			int childNodes=rNode->getChildCount();
			for (int i=0;i<childNodes;i++) {
				Node *rNodeChild=rNode->getNodeFromIndex(i);
				if (rNodeChild) {
					addNode(rQTreeWidgetItem_found, rNodeChild, rNodeInfoType);
				}
			}
		}
	}
}

void SceneTreeDock::updateAllNodeNamesX(QTreeWidgetItem *parent) {
	if (parent) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(parent);
		if (rNode) {
			setNodeName(parent, rNode->getName());
		}
		int count=parent->childCount();
		for (int i=0;i<count;i++) {
			QTreeWidgetItem *r=parent->child(i);
			updateAllNodeNamesX(r);
		}
	}
}

void SceneTreeDock::updateAllNodeNames(NodeInfoType rNodeInfoType) {
	if (rNodeInfoType==NodeInfoType::Scene) {
		updateAllNodeNamesX(ui->treeWidget->topLevelItem(0));
	} else {
		updateAllNodeNamesX(ui->treeWidgetResources->topLevelItem(0));
	}
}

bool SceneTreeDock::updateNodeNameX(QTreeWidgetItem *parent, Node* rUpdateNode) {
	bool updated=false;
	if (parent) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(parent);
		if (rNode && rNode==rUpdateNode) {
			setNodeName(parent, rNode->getName());
			updated=true;
		}
		if (!updated) {
			int count=parent->childCount();
			for (int i=0;i<count && !updated;i++) {
				QTreeWidgetItem *r=parent->child(i);
				updated=updateNodeNameX(r, rUpdateNode);
			}
		}
	}
	return updated;
}

void SceneTreeDock::updateNodeName(Node* rUpdateNode, NodeInfoType rNodeInfoType) {
	if (rNodeInfoType==NodeInfoType::Scene) {
		updateNodeNameX(ui->treeWidget->topLevelItem(0), rUpdateNode);
	} else {
		updateNodeNameX(ui->treeWidgetResources->topLevelItem(0), rUpdateNode);
	}
}

QTreeWidgetItem* SceneTreeDock::addNode(QTreeWidgetItem *parent, Node* rNode, NodeInfoType rNodeInfoType) {
	if (rNodeInfoType==NodeInfoType::Scene) {
		return addNodeX(parent, rNode, ui->treeWidget, true);
	} else {
		return addNodeX(parent, rNode, ui->treeWidgetResources, false);
	}
}

void SceneTreeDock::setNodeName(QTreeWidgetItem *item, const string& rName) {
	if (item) {
		item->setText(1,QString::fromStdString(rName));
	}
}

QTreeWidgetItem* SceneTreeDock::addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget, bool expandTree) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
		if (parent) {
			r=new QTreeWidgetItem(parent);
		} else {
			r=new QTreeWidgetItem(rQTreeWidget);
		}
		// Type + ID
		TreeUtil::setTypeNameToTreeItem(r,rNode);
		// Name
		TreeUtil::setNodeNameToTreeItem(r, rNode);
		//r->setText(1,QString::fromStdString(rNode->getName()));
		TreeUtil::setNodeToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		if (parent) {
			parent->addChild(r);
			if (expandTree) {
				parent->setExpanded(true);
			}
		} else {
			rQTreeWidget->addTopLevelItem(r);
			r->setExpanded(true);
		}
		if (rNode->getNodeType()==NodeType::Texture) {
			QPushButton *rButton=new QPushButton("T",rQTreeWidget);
			rButton->setToolTip(tr("Edit Frames..."));
			rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
			connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
				TextureFrameEditor *rTextureFrameEditor=new TextureFrameEditor(rNode, this);
				rTextureFrameEditor->show();
			});
			rQTreeWidget->setItemWidget(r, 2, rButton);
		} else if (rNode->getNodeType()==NodeType::Animation) {
			QPushButton *rButton=new QPushButton("A",rQTreeWidget);
			rButton->setToolTip(tr("Edit Animation Frames..."));
			rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
			connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
				AnimationFrameEditor *rAnimationFrameEditor=new AnimationFrameEditor(rNode, this);
				rAnimationFrameEditor->show();
			});
			rQTreeWidget->setItemWidget(r, 2, rButton);
		}


		if (rNode->getChildCount()>0) {
			unsigned long count=rNode->getChildCount();
			for (unsigned long i=0;i<count;i++) {
				Node *rNodeChild=rNode->getNodeFromIndex(i);
				if (rNodeChild) {
					addNodeX(r, rNodeChild, rQTreeWidget, expandTree);
				}
			}
		}
	}
	return r;
}

void SceneTreeDock::updateSceneOrResourceDropdownWithCurrent(NodeInfoType rNodeInfoType) {
	int i=0;
	if (rNodeInfoType==NodeInfoType::Scene) {
		ui->sceneNameOpm->clear();
	} else if (rNodeInfoType==NodeInfoType::Resource) {
		ui->resourceNameOpm->clear();
	}
	vector<string> v=ProjectContext::getInstance().getFileNamesForType(rNodeInfoType);

	for (auto& s : v) {
		if (rNodeInfoType==NodeInfoType::Scene) {
			ui->sceneNameOpm->addItem(QString::fromStdString(s));
			if (GuiContext::getInstance().getCurrentScene() && GuiContext::getInstance().getCurrentScene()->getName()==s) {
				ui->sceneNameOpm->setCurrentIndex(i);
			}
		} else if (rNodeInfoType==NodeInfoType::Resource) {
			ui->resourceNameOpm->addItem(QString::fromStdString(s));
			if (GuiContext::getInstance().getCurrentResource() && GuiContext::getInstance().getCurrentResource()->getName()==s) {
				ui->resourceNameOpm->setCurrentIndex(i);
			}
		}
		i++;
	}
}

void SceneTreeDock::switchToSceneOrResource(Node *rNode, NodeInfoType rNodeInfoType) {
	if (rNodeInfoType==NodeInfoType::Scene) {
		clearScene();
		if (rNode) {
			setSceneEditable(true);
			addNode(nullptr, rNode, rNodeInfoType);
		} else {
			setSceneEditable(false);
		}
	} else if (rNodeInfoType==NodeInfoType::Resource) {
		clearResource();
		if (rNode) {
			setResourceEditable(true);
			addNode(nullptr, rNode, rNodeInfoType);
		} else {
			setResourceEditable(false);
		}
	}
}

void SceneTreeDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		GuiContext::getInstance().switchProperties(rNode, NodeInfoType::Scene);
	}
}

void SceneTreeDock::on_treeWidgetResources_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		if (rNode) {
			if (rNode->getNodeType()==NodeType::Texture) {
				QMessageBox::information(this, "Brickleedit", "Das ist eine Texture");
			}
		}
	}
}

void SceneTreeDock::on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		GuiContext::getInstance().switchProperties(rNode, NodeInfoType::Resource);
	}
}

void SceneTreeDock::on_tabWidget_currentChanged(int index)
{
	NodeInfoType rNodeInfoType=NodeInfoType::Scene;
	Node* rNode=nullptr;
	if (index==0) {
		rNodeInfoType=NodeInfoType::Scene;
	} else {
		rNodeInfoType=NodeInfoType::Resource;
	}
	rNode=getSelectedNode(rNodeInfoType);
	if (rNode) {
		// do not switch if in the current tab nothing is selected/available -> do not remove the current property if anyone is currently outlined
		GuiContext::getInstance().switchProperties(rNode, rNodeInfoType);
	}
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(const QString &arg1)
{
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(const QString &arg1)
{
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(int index)
{
	string name="";
	if (index>=0) {
		name=ui->sceneNameOpm->itemText(index).toStdString();
	}
	GuiContext::getInstance().setCurrentScene(name);
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(int index)
{
	string name="";
	if (index>=0) {
		name=ui->resourceNameOpm->itemText(index).toStdString();
	}
	GuiContext::getInstance().setCurrentResource(name);
}
