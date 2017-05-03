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

void SceneTreeDock::on_newNode_clicked()
{
	if (newSceneNodeDialog==nullptr) {
		newSceneNodeDialog=new NewSceneNodeDialog(this);
		QObject::connect(newSceneNodeDialog, &NewSceneNodeDialog::newNodeSelected, this, &SceneTreeDock::onNewNodeSelected);
	}
	newSceneNodeDialog->show();
}

void SceneTreeDock::onNewNodeSelected(QString rNodeTypeName) {
	QTreeWidgetItem *r=new QTreeWidgetItem(ui->treeWidget);
	r->setText(0,rNodeTypeName);
	ui->treeWidget->addTopLevelItem(r);
	//r->addChild(r,"Child A", "X");
}
