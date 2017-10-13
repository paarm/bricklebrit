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
	static void setTypeNameToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setNodeNameToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
	static string getNodeNameWithId(Node *rNode);
	static void fillTreeWidgetWithTexturesFromResource(QTreeWidget *rQTreeWidget, const string &rResourceName, bool addTextures, bool addAnimations);
	static void setPixmapToTreeItem(QTreeWidget *rQTreeWidget, QTreeWidgetItem *r, QPixmap& rPixmap);
	static QTreeWidgetItem* getTreeWidgetItemFromNode(QTreeWidget *rQTreeWidget, Node *rNode);
	static QTreeWidgetItem* getTreeWidgetItemFromNode_Parent(QTreeWidget *rQTreeWidget, QTreeWidgetItem* rParent, Node *rNode);
};

#endif // TREEUTIL_H
