#include "brushdock.h"
#include "ui_brushdock.h"
#include "brushselection.h"
#include "../previewimageutil.h"

BrushDock::BrushDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::BrushDock)
{
	ui->setupUi(this);
	setBrushEnabled(false);
}

BrushDock::~BrushDock()
{
	delete ui;
}

void BrushDock::on_selectBrush_clicked()
{
	BrushSelection *rBrushSelection=new BrushSelection(mSelectedItem.resourceName, getSelectedBrushNode(), this);
	rBrushSelection->show();
}

void BrushDock::clearBrush() {
	ui->previewImage->setPixmap(QPixmap());
	ui->scale->setValue(1.0);
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
	ui->rotate->setValue(0);
	ui->currentBrush->setText("");
}

void BrushDock::setBrushEnabled(bool enabled) {
	if (!enabled) {
		clearBrush();
	}
	ui->selectBrush->setEnabled(enabled);
	ui->scale->setEnabled(enabled);
	ui->sizeX->setEnabled(enabled);
	ui->sizeY->setEnabled(enabled);
	ui->rotate->setEnabled(enabled);
}

void BrushDock::setAsBrush(SelectedItem rSelectedItem) {
	if (rSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(rSelectedItem, 100, 100);
		ui->previewImage->setPixmap(pixmap);
		mSelectedItem=rSelectedItem;
	}
}

SelectedItem& BrushDock::getSelectedBrush() {
	return mSelectedItem;
}

Node* BrushDock::getSelectedBrushNode() {
	if (mSelectedItem.rNodeAnimationFrame) {
		return mSelectedItem.rNodeAnimationFrame;
	} else if (mSelectedItem.rNodeTextureFrame) {
		return mSelectedItem.rNodeTextureFrame;
	} else {
		return mSelectedItem.rNodeTexture;
	}
	return nullptr;
}
