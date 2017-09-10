#ifndef TREEUTIL_H
#define TREEUTIL_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>

Q_DECLARE_METATYPE(Node*);

class TreeUtil {
public:
	static Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	static void setNodeDataToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setTypeNameToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setNodeNameToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
};

#endif // TREEUTIL_H
