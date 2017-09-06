#include "scenetreedock.h"
#include "ui_scenetreedock.h"
#include "../guicontext.h"

SceneTreeDock::SceneTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SceneTreeDock)
{
	ui->setupUi(this);
	ui->sceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());
	ui->sceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenScene());

	ui->resourceMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewResource());
	ui->resourceMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenResource());

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

Node* SceneTreeDock::getSelectedNode(NodeInfoType rNodeInfoType) {
	QTreeWidgetItem*r=getSelectedItem(rNodeInfoType);
	Node *rv=nullptr;
	if (r) {
		rv=getNodeFromTreeItem(r);
	}
	return rv;
}

Node* SceneTreeDock::getNodeFromTreeItem(QTreeWidgetItem* r) {
	Node *rv=nullptr;
	if (r) {
		QVariant v=r->data(0,Qt::UserRole);
		rv=v.value<Node*>();
	}
	return rv;
}

void SceneTreeDock::setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode) {
	if (r && rNode) {
		QVariant v;
		v.setValue(rNode);
		r->setData(0, Qt::UserRole, v);
	}
}

QTreeWidgetItem* SceneTreeDock::searchNode(QTreeWidgetItem *parent, Node* rNode) {
	int count=parent->childCount();
	QTreeWidgetItem* rv=nullptr;
	for (int i=0;i<count;i++) {
		QTreeWidgetItem *r=parent->child(i);
		// test something
	}
	if (!rv) {
		for (int i=0;i<count && !rv;i++) {
			QTreeWidgetItem *r=parent->child(i);
			rv=searchNode(r, rNode);
		}
	}

	return rv;
}

void SceneTreeDock::updateAllNodeNamesX(QTreeWidgetItem *parent) {
	if (parent) {
		Node* rNode=getNodeFromTreeItem(parent);
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
		Node* rNode=getNodeFromTreeItem(parent);
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
		return addNodeX(parent, rNode, ui->treeWidget);
	} else {
		return addNodeX(parent, rNode, ui->treeWidgetResources);
	}
}

void SceneTreeDock::setNodeName(QTreeWidgetItem *item, const string& rName) {
	if (item) {
		item->setText(1,QString::fromStdString(rName));
	}
}

QTreeWidgetItem* SceneTreeDock::addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
		if (parent) {
			r=new QTreeWidgetItem(parent);
		} else {
			r=new QTreeWidgetItem(rQTreeWidget);
		}
		// Type + ID
		string rType=getNodeTypeAsString(rNode->getNodeType());
		rType+="("+std::to_string(rNode->getId())+")";
		r->setText(0, QString::fromStdString(rType));
		// Name
		setNodeName(r, rNode->getName());
		//r->setText(1,QString::fromStdString(rNode->getName()));
		setNodeToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		if (parent) {
			parent->addChild(r);
			parent->setExpanded(true);
		} else {
			rQTreeWidget->addTopLevelItem(r);
			r->setExpanded(true);
		}
		if (rNode->getNodeType()==NodeType::Texture) {
			QPushButton *rButton=new QPushButton("...",rQTreeWidget);
			rButton->setToolTip(tr("Edit Frames..."));
			rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
			rQTreeWidget->setItemWidget(r, 2, rButton);
		}

		if (rNode->getChildCount()>0) {
			unsigned long count=rNode->getChildCount();
			for (unsigned long i=0;i<count;i++) {
				Node *rNodeChild=rNode->getNodeFromIndex(i);
				if (rNodeChild) {
					addNodeX(r, rNodeChild, rQTreeWidget);
				}
			}
		}
	}
	return r;
}

void SceneTreeDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	Node* rNode=getNodeFromTreeItem(item);
	GuiContext::getInstance().switchProperties(rNode, NodeInfoType::Scene);
}

void SceneTreeDock::on_treeWidgetResources_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	Node* rNode=getNodeFromTreeItem(item);
	if (rNode) {
		if (rNode->getNodeType()==NodeType::Texture) {
			QMessageBox::information(this, "Brickleedit", "Das ist eine Texture");
		}
	}
}

void SceneTreeDock::on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column)
{
	Node* rNode=getNodeFromTreeItem(item);
	GuiContext::getInstance().switchProperties(rNode, NodeInfoType::Resource);
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
