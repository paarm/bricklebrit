#include "newscenenodedialog.h"
#include "ui_newscenenodedialog.h"

NewSceneNodeDialog::NewSceneNodeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSceneNodeDialog)
{
	ui->setupUi(this);
	QListWidgetItem *rSprite=new QListWidgetItem();
	rSprite->setIcon(QIcon(":/icons/new.png"));
	rSprite->setText("Sprite");
	ui->listWidget->addItem(rSprite);
}

NewSceneNodeDialog::~NewSceneNodeDialog()
{
	delete ui;
}

void NewSceneNodeDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
	QList<QListWidgetItem*>itemList=ui->listWidget->selectedItems();
	for (QListWidgetItem* item : itemList) {
		item->setBackgroundColor(QColor::fromRgbF(30,30,30));
		emit newNodeSelected(item->text());
	}
	close();
	//QListWidgetItem *item = ui->listWidget->currentItem();
}
