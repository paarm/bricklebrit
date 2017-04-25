#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include "../guicontext.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewProjectDialog)
{
	ui->setupUi(this);
}

void NewProjectDialog::on_buttonBox_clicked(QAbstractButton *button) {
	ProjectContext::getInstance().createNewProject(getProjectName().toStdString());
	GuiContext::getInstance().setWindowTitle(getProjectName());
}

NewProjectDialog::~NewProjectDialog()
{
	delete ui;
}

void NewProjectDialog::init() {
	setProjectName("");
}

void NewProjectDialog::setProjectName(const QString& rProjectName) {
	ui->projectName->setText(rProjectName);
}

QString NewProjectDialog::getProjectName() {
	return ui->projectName->text();
}
