#include "scenetreedock.h"
#include "ui_scenetreedock.h"

SceneTreeDock::SceneTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SceneTreeDock)
{
	ui->setupUi(this);
}

SceneTreeDock::~SceneTreeDock()
{
	delete ui;
}
