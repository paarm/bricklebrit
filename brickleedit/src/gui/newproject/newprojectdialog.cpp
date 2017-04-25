#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include "../guicontext.h"

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
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
												 "Projects",
												 QFileDialog::ShowDirsOnly
												 | QFileDialog::DontResolveSymlinks);
	setProjectPath(dir);
}


void NewProjectDialog::on_okButton_clicked()
{
	if (getProjectName().toStdString().size()==0 || getProjectPath().size()==0) {
		QMessageBox::warning(this, tr("Error"), tr("Project Name and Project Path are required"), QMessageBox::Ok);
	} else {
		QString pathWithFile = QDir(getProjectPath()).filePath("brickleroot.txt");
		QFileInfo check_file(pathWithFile);
		// check if file exists and if yes: Is it really a file and no directory?
		if (check_file.exists() && check_file.isFile()) {
			QMessageBox::warning(this, tr("Error"), tr("A Project exists already in this folder"), QMessageBox::Ok);
		} else {
			if (!ProjectContext::getInstance().createNewProject(getProjectName().toStdString(), getProjectPath().toStdString(), pathWithFile.toStdString())) {
				QMessageBox::warning(this, tr("Error"), tr("Could not create Project"), QMessageBox::Ok);
			} else {
				GuiContext::getInstance().setWindowTitle(getProjectName());
				close();
			}
		}
	}
}

void NewProjectDialog::on_cancelButton_clicked()
{
	close();
}
