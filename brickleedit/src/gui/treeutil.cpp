#include "treeutil.h"

Node* TreeUtil::getNodeFromTreeItem(QTreeWidgetItem* r) {
	Node *rv=nullptr;
	if (r) {
		QVariant v=r->data(0,Qt::UserRole);
		rv=v.value<Node*>();
	}
	return rv;
}

void TreeUtil::setNodeDataToTreeItem(QTreeWidgetItem* r, Node *rNode) {
	if (r && rNode) {
		QVariant v;
		v.setValue(rNode);
		r->setData(0, Qt::UserRole, v);
	}
}

void TreeUtil::setTypeNameToTreeItem(QTreeWidgetItem* r, Node *rNode) {
	if (r && rNode) {
		string rType=getNodeTypeAsString(rNode->getNodeType());
		rType+="("+std::to_string(rNode->getId())+")";
		r->setText(0, QString::fromStdString(rType));
	}
}

void TreeUtil::setNodeNameToTreeItem(QTreeWidgetItem* r, Node* rNode) {
	if (r && rNode) {
		r->setText(1,QString::fromStdString(rNode->getName()));
	}
}

void TreeUtil::setNodeToTreeItem(QTreeWidgetItem* r, Node* rNode) {
	if (r && rNode) {
		TreeUtil::setTypeNameToTreeItem(r,rNode);
		TreeUtil::setNodeNameToTreeItem(r,rNode);
		TreeUtil::setNodeDataToTreeItem(r,rNode);
	}
}
