#ifndef SCENETREEDOCK_H
#define SCENETREEDOCK_H

#include <QDockWidget>

namespace Ui {
class SceneTreeDock;
}

class SceneTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit SceneTreeDock(QWidget *parent = 0);
	~SceneTreeDock();

private:
	Ui::SceneTreeDock *ui;
};

#endif // SCENETREEDOCK_H
