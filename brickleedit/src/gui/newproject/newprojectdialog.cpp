#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include "../guicontext.h"
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>


NewProjectDialog::NewProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewProjectDialog)
{
	ui->setupUi(this);
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
	return ui->projectName->text().trimmed();
}

void NewProjectDialog::setProjectPath(const QString& rProjectPath) {
	ui->projectPath->setText(rProjectPath);
}

QString NewProjectDialog::getProjectPath() {
	return ui->projectPath->text().trimmed();
}

void NewProjectDialog::on_choosePath_clicked() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select new Project path"),
												 "Projects",
												 QFileDialog::ShowDirsOnly
												 | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		setProjectPath(dir);
	}
}

void NewProjectDialog::on_okButton_clicked()
{
	if (getProjectName().toStdString().size()==0 || getProjectPath().size()==0) {
		QMessageBox::warning(this, tr("Error"), tr("Project Name and Project Path are required"), QMessageBox::Ok);
	} else {
		QString rProjectName=getProjectName();
		if (!rProjectName.endsWith("brprj")) {
			rProjectName+=".brprj";
		}
		QString pathWithFile = QDir(getProjectPath()).filePath(rProjectName);
		QFileInfo check_file(pathWithFile);
		// check if file exists and if yes: Is it really a file and no directory?
		if (check_file.exists() && check_file.isFile()) {
			QMessageBox::warning(this, tr("Error"), tr("A Project exists already in this folder"), QMessageBox::Ok);
		} else {
			if (!GuiContext::getInstance().createNewProject(getProjectPath().toStdString(), rProjectName.toStdString())) {
				QMessageBox::warning(this, tr("Error"), tr("Could not create Project"), QMessageBox::Ok);
			} else {
				close();
			}
		}
	}
}

void NewProjectDialog::on_cancelButton_clicked()
{
	close();
}
