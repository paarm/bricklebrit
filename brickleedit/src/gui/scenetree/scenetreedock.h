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
	QTreeWidgetItem* addNode(QTreeWidgetItem *parent, Node* rNode);
	void setNodeToTreeItem(QTreeWidgetItem* r, Node *rNode);
	Node* getNodeFromTreeItem(QTreeWidgetItem* r);
	void clear();
private slots:
	void on_newNode_clicked();
private:
	Ui::SceneTreeDock *ui;
	NewSceneNodeDialog *newSceneNodeDialog=nullptr;
};

#endif // SCENETREEDOCK_H
