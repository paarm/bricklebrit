#include "newscenedialog.h"
#include "ui_newscenedialog.h"
#include "../guicontext.h"
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

NewSceneDialog::NewSceneDialog(bool rIsResource, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSceneDialog),
	mIsResource(rIsResource)
{
	ui->setupUi(this);
}

NewSceneDialog::~NewSceneDialog()
{
	delete ui;
}

QString NewSceneDialog::getName() {
	return ui->name->text().trimmed();
}

void NewSceneDialog::setName(QString rName) {
	ui->name->setText(rName);
}

void NewSceneDialog::on_okButton_clicked()
{
	if (getName().toStdString().size()==0) {
		QMessageBox::warning(this, tr("Error"), tr("Name required"), QMessageBox::Ok);
	} else {
		string rFullName="";
		if (!mIsResource) {
			rFullName = DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getScenesSubPath(),getName().toStdString());
			rFullName+=".brscn";
			if (ProjectContext::getInstance().getSceneInfoByName(rFullName)) {
				QMessageBox::warning(this, tr("Error"), tr("A Scene with this Name exists already"), QMessageBox::Ok);
			} else if (!GuiContext::getInstance().createNewScene(rFullName, false)) {
				QMessageBox::warning(this, tr("Error"), tr("Could not create Scene"), QMessageBox::Ok);
			} else {
				close();
			}
		} else {
			rFullName = DirUtil::concatPath(ProjectContext::getInstance().getNodeProject()->getResourcesSubPath(),getName().toStdString());
			rFullName+=".brres";
			if (ProjectContext::getInstance().getResourceInfoByName(rFullName)) {
				QMessageBox::warning(this, tr("Error"), tr("A Resource with this Name exists already"), QMessageBox::Ok);
			} else if (!GuiContext::getInstance().createNewResource(rFullName, false)) {
				QMessageBox::warning(this, tr("Error"), tr("Could not create Resource"), QMessageBox::Ok);
			} else {
				close();
			}
		}
	}
}

void NewSceneDialog::on_cancelButton_clicked()
{
	close();
}
