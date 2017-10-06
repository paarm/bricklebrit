#include "brushselection.h"
#include "ui_brushselection.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include "../previewimageutil.h"

BrushSelection::BrushSelection(const string& rResourceName, Node *rBrushNode, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BrushSelection),
	mNode(rBrushNode)
{
	ui->setupUi(this);
	mTexturePicker=new TexturePicker(ui->treeWidget);

	string currentResourceName="";
	if (mNode) {
		currentResourceName=rResourceName;
		mSelectedItem=mTexturePicker->initialize(currentResourceName, mNode);
		drawPreviewImage();
	} else if (GuiContext::getInstance().getCurrentResource()) {
		currentResourceName=GuiContext::getInstance().getCurrentResource()->getName();
		mTexturePicker->switchToResource(currentResourceName);
	}

	vector<string> v=ProjectContext::getInstance().getResourceNames();
	int i=0;
	for (auto& s : v) {
		ui->resourceNameOpm->addItem(QString::fromStdString(s));
		if (s==currentResourceName) {
			ui->resourceNameOpm->setCurrentIndex(i);
		}
		i++;
	}
}

void BrushSelection::drawPreviewImage() {
	if (mSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(mSelectedItem, 200, 200);
		ui->previewImage->setPixmap(pixmap);
	}
}

BrushSelection::~BrushSelection()
{
	delete ui;
	delete mTexturePicker;
}

void BrushSelection::on_resourceNameOpm_activated(const QString &arg1)
{
	ignoreparam(arg1);
	QString rCurrentResource=ui->resourceNameOpm->currentText();
	mTexturePicker->switchToResource(rCurrentResource.toStdString());
}

void BrushSelection::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	ignoreparam(column);
	Node *rNode=TreeUtil::getNodeFromTreeItem(item);
	mSelectedItem=mTexturePicker->setSelectedNode(rNode);
	drawPreviewImage();
}

void BrushSelection::on_cancelButton_clicked()
{
	this->deleteLater();
}

void BrushSelection::on_BrushSelection_rejected()
{
	this->deleteLater();
}

void BrushSelection::on_okButton_clicked()
{
	GuiContext::getInstance().getMainWindow().getBrushDock().setAsBrush(mSelectedItem);
	this->deleteLater();
}
