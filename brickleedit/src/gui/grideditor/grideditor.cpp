#include "grideditor.h"
#include "ui_grideditor.h"
#include "../guicontext.h"

GridEditor::GridEditor(int gridX, int gridY, int offsetX, int offsetY, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GridEditor)
{
	ui->setupUi(this);
	ui->gridX->setValue(gridX);
	ui->gridY->setValue(gridY);
	ui->offsetX->setValue(offsetX);
	ui->offsetY->setValue(offsetY);
}

GridEditor::~GridEditor()
{
	delete ui;
}

void GridEditor::on_cancelButton_clicked()
{
	deleteLater();
}

void GridEditor::on_okButton_clicked()
{
	GuiContext::getInstance().updateGridSettings(ui->gridX->value(), ui->gridY->value(), ui->offsetX->value(), ui->offsetY->value());
	deleteLater();
}

void GridEditor::on_GridEditor_rejected()
{
	deleteLater();
}
