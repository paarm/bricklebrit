#ifndef SCENETREEDOCK_H
#define SCENETREEDOCK_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include "newscenenode/newscenenodedialog.h"
#include <bricklelib.h>

namespace Ui {
class SceneTreeDock;
}

Q_DECLARE_METATYPE(Node*);

class SceneTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit SceneTreeDock(QWidget *parent = 0);
	~SceneTreeDock();
	QTreeWidgetItem* getSelectedItem();
	Node* getSelectedNode();

	QTreeWidgetItem* searchNode(QTreeWidgetItem *parent, Node* rNode);
	QTreeWidgetItem* addSceneNode(QTreeWidgetItem *parent, Node* rNode);
	QTreeWidgetItem* addResourceNode(QTreeWidgetItem *parent, Node* rNode);

	void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
	Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	void clearScene();
	void clearResource();
	void setSceneEditable(bool isEditable);
	void setResourceEditable(bool isEditable);
	void setProjectAvailable(bool isActive);
private slots:
	void on_newNode_clicked();
	//void onSelectionChanged();
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::SceneTreeDock *ui;
	NewSceneNodeDialog *newSceneNodeDialog=nullptr;
	QTreeWidgetItem* addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget);
};

#endif // SCENETREEDOCK_H
