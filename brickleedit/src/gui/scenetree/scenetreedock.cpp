#include "scenetreedock.h"
#include "ui_scenetreedock.h"
#include "../guicontext.h"

SceneTreeDock::SceneTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SceneTreeDock)
{
	ui->setupUi(this);
	setSceneEditable(false);
}

SceneTreeDock::~SceneTreeDock()
{
	delete ui;
}

void SceneTreeDock::clear() {
	ui->treeWidget->clear();
	setSceneEditable(false);
}

void SceneTreeDock::setSceneEditable(bool isEditable) {
	ui->newNode->setEnabled(isEditable);
}

void SceneTreeDock::on_newNode_clicked()
{
	if (newSceneNodeDialog==nullptr) {
		newSceneNodeDialog=new NewSceneNodeDialog(this);
		QObject::connect(newSceneNodeDialog, &NewSceneNodeDialog::newNodeSelected, &GuiContext::getInstance(), &GuiContext::onCreateNewNode);
	}
	newSceneNodeDialog->show();
}

QTreeWidgetItem* SceneTreeDock::getSelectedItem() {
	QTreeWidgetItem* rv=nullptr;
	QList<QTreeWidgetItem*>selectedItems=ui->treeWidget->selectedItems();
	if (!selectedItems.isEmpty()) {
		rv=selectedItems[0];
	}
	return rv;
}

Node* SceneTreeDock::getSelectedNode() {
	QTreeWidgetItem*r=getSelectedItem();
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

QTreeWidgetItem* SceneTreeDock::addNode(QTreeWidgetItem *parent, Node* rNode) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
		if (parent) {
			r=new QTreeWidgetItem(parent);
		} else {
			r=new QTreeWidgetItem(ui->treeWidget);
		}
		// Type + ID
		string rType=getNodeTypeAsString(rNode->getNodeType());
		rType+="("+std::to_string(rNode->getId())+")";
		r->setText(0, QString::fromStdString(rType));
		// Name
		r->setText(1,QString::fromStdString(rNode->getName()));
		setNodeToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		if (parent) {
			parent->addChild(r);
			parent->setExpanded(true);
		} else {
			ui->treeWidget->addTopLevelItem(r);
			r->setExpanded(true);
		}
		if (rNode->getChildCount()>0) {
			unsigned long count=rNode->getChildCount();
			for (unsigned long i=0;i<count;i++) {
				Node *rNodeChild=rNode->getNodeFromIndex(i);
				if (rNodeChild) {
					addNode(r, rNodeChild);
				}
			}
		}
	}
	return r;
}



void SceneTreeDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	Node* rNode=getNodeFromTreeItem(item);
	GuiContext::getInstance().switchProperties(true, rNode);
}
