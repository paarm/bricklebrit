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

	QTreeWidgetItem* searchTreeWidgetItemByNode(QTreeWidgetItem *parent, Node* rNode);
	QTreeWidgetItem* addNode(QTreeWidgetItem *parent, Node* rNode, NodeInfoType rNodeInfoType);

	void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
	Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	void clearScene();
	void clearResource();
	void setSceneEditable(bool isEditable);
	void setResourceEditable(bool isEditable);
	void setProjectAvailable(bool isActive);
	int getActiveTreeTabIndex();
	void updateAllNodeNames(NodeInfoType rNodeInfoType);
	void updateNodeName(Node* rUpdateNode, NodeInfoType rNodeInfoType);
	void updateChildNodes(Node* rNode, NodeInfoType rNodeInfoType);

private slots:
	void on_newNode_clicked();
	//void onSelectionChanged();
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column);

	void on_tabWidget_currentChanged(int index);

	void on_newResource_clicked();

	void on_treeWidgetResources_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	Ui::SceneTreeDock *ui;
	NewSceneNodeDialog *newSceneNodeDialog=nullptr;
	NewSceneNodeDialog *newResourceNodeDialog=nullptr;
	QTreeWidget* getTreeWidgetFromNodeInfoType(NodeInfoType rNodeInfoType);
	QTreeWidgetItem* addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget);
	void updateAllNodeNamesX(QTreeWidgetItem *parent);
	bool updateNodeNameX(QTreeWidgetItem *parent, Node* rUpdateNode);
	void setNodeName(QTreeWidgetItem *item, const string& rName);
	void deleteChilds(QTreeWidgetItem *parent);

};

#endif // SCENETREEDOCK_H
