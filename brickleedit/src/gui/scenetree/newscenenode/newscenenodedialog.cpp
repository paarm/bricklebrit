#include "newscenenodedialog.h"
#include "ui_newscenenodedialog.h"

NewSceneNodeDialog::NewSceneNodeDialog(NodeInfoType rNodeInfoType, QWidget *parent) :
	QDialog(parent),
	mNodeInfoType(rNodeInfoType),
	ui(new Ui::NewSceneNodeDialog)
{
	ui->setupUi(this);
	if (rNodeInfoType==NodeInfoType::Scene) {
		QListWidgetItem *rSprite=new QListWidgetItem();
		rSprite->setIcon(QIcon(":/icons/new.png"));
		rSprite->setText("Sprite");
		ui->listWidget->addItem(rSprite);
	} else {
		QListWidgetItem *r=new QListWidgetItem();
		r->setIcon(QIcon(":/icons/new.png"));
		r->setText("Texture");
		ui->listWidget->addItem(r);
		r=new QListWidgetItem();
		r->setIcon(QIcon(":/icons/new.png"));
		r->setText("Animation");
		ui->listWidget->addItem(r);
	}
}

NewSceneNodeDialog::~NewSceneNodeDialog()
{
	delete ui;
}

void NewSceneNodeDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
	QList<QListWidgetItem*>itemList=ui->listWidget->selectedItems();
	for (QListWidgetItem* item : itemList) {
		//item->setBackgroundColor(QColor::fromRgbF(30,30,30));
		emit newNodeSelected(item->text(), mNodeInfoType);
	}
	close();
	//QListWidgetItem *item = ui->listWidget->currentItem();
}

void NewSceneNodeDialog::on_cancelButton_clicked()
{
	close();
}
