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
	QTreeWidgetItem* getSelectedItem(NodeInfoType rNodeInfoType);
	Node* getSelectedNode(NodeInfoType rNodeInfoType);

	QTreeWidgetItem* searchNode(QTreeWidgetItem *parent, Node* rNode);
	QTreeWidgetItem* addNode(QTreeWidgetItem *parent, Node* rNode, NodeInfoType rNodeInfoType);

	void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
	Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	void clearScene();
	void clearResource();
	void setSceneEditable(bool isEditable);
	void setResourceEditable(bool isEditable);
	void setProjectAvailable(bool isActive);
	int getActiveTreeTabIndex();

private slots:
	void on_newNode_clicked();
	//void onSelectionChanged();
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column);

	void on_tabWidget_currentChanged(int index);

	void on_newResource_clicked();

private:
	Ui::SceneTreeDock *ui;
	NewSceneNodeDialog *newSceneNodeDialog=nullptr;
	NewSceneNodeDialog *newResourceNodeDialog=nullptr;

	QTreeWidgetItem* addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget);
};

#endif // SCENETREEDOCK_H
