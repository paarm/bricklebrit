#ifndef TREEUTIL_H
#define TREEUTIL_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>
#include <QPixmap>


Q_DECLARE_METATYPE(Node*);

class TreeUtil {
public:
	static Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	static void setNodeDataToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setTypeNameToTreeItem(int columnIndex, QTreeWidgetItem* r, Node *rNode);
	static void setNodeNameToTreeItem(int columnIndex, QTreeWidgetItem* r, Node *rNode);
	static string getNodeNameWithId(Node *rNode);
	static void fillTreeWidgetWithTexturesFromResource(QTreeWidget *rQTreeWidget, const string &rResourceName, bool addTextures, bool addAnimations);
	static QTreeWidgetItem* getTreeWidgetItemFromNode(QTreeWidget *rQTreeWidget, Node *rNode);
	static QTreeWidgetItem* getTreeWidgetItemFromNode_Parent(QTreeWidget *rQTreeWidget, QTreeWidgetItem* rParent, Node *rNode);
};

#endif // TREEUTIL_H
