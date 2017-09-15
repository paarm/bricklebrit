#ifndef TREEUTIL_H
#define TREEUTIL_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>
#include <QImage>

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
	static void setPixmapToTreeItem(QTreeWidget *rQTreeWidget, QTreeWidgetItem *r, QImage& rImageSrc, int srcX, int srcY, int srcWidht, int srcHeight, int destWidth, int destHeight);
};

#endif // TREEUTIL_H
