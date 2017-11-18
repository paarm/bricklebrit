#ifndef SELECTIONDOCK_H
#define SELECTIONDOCK_H

#include <QDockWidget>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <bricklelib.h>

namespace Ui {
class SelectionDock;
}

class SelectionDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit SelectionDock(QWidget *parent = 0);
	~SelectionDock();
	void removeNode(Node *rNode);
	void removeAllNodes();
	void addNode(Node *rNode);
    void addNodes(vector<Node*> &v);
private slots:
	void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_deselectOther_clicked();

	void on_setAsCanvas_clicked();

    void on_reparentNodes_clicked();

private:
	Ui::SelectionDock *ui;
	void updateLatestSelected();
    QTreeWidgetItem*  prepareNodeIntern(Node *rNode);
    QPushButton* prepareDeselectButton(Node *rNode);
    QPushButton* prepareEditSpriteButton(Node *rNode);


};

#endif // SELECTIONDOCK_H
