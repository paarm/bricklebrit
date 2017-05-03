#ifndef SCENETREEDOCK_H
#define SCENETREEDOCK_H

#include <QDockWidget>
#include "newscenenode/newscenenodedialog.h"

namespace Ui {
class SceneTreeDock;
}

class SceneTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit SceneTreeDock(QWidget *parent = 0);
	~SceneTreeDock();

private slots:
	void on_newNode_clicked();
	void onNewNodeSelected(QString rNodeTypeName);
private:
	Ui::SceneTreeDock *ui;
	NewSceneNodeDialog *newSceneNodeDialog=nullptr;
};

#endif // SCENETREEDOCK_H
