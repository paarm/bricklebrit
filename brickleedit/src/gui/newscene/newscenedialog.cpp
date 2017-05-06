#include "newscenedialog.h"
#include "ui_newscenedialog.h"
#include "../guicontext.h"

NewSceneDialog::NewSceneDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSceneDialog)
{
	ui->setupUi(this);
}

NewSceneDialog::~NewSceneDialog()
{
	delete ui;
}

void NewSceneDialog::setSceneName(const QString& rSceneName) {
	ui->sceneName->setText(rSceneName);
}

QString NewSceneDialog::getSceneName() {
	return ui->sceneName->text().trimmed();
}

void NewSceneDialog::setScenePath(const QString& rScenePath) {
	ui->scenePath->setText(rScenePath);
}

QString NewSceneDialog::getScenePath() {
	return ui->scenePath->text().trimmed();
}

void NewSceneDialog::on_okButton_clicked()
{
	QString rScenePathAbs=GuiContext::getInstance().fromVirtualPath(getScenePath());

	if (getSceneName().toStdString().size()==0 || rScenePathAbs.size()==0) {
		QMessageBox::warning(this, tr("Error"), tr("Scene Name and Scene Path are required"), QMessageBox::Ok);
	} else {
		QString pathWithFile = QDir(rScenePathAbs).filePath(getSceneName()+".brscn");
		QFileInfo check_file(pathWithFile);
		// check if file exists and if yes: Is it really a file and no directory?
		if (check_file.exists() && check_file.isFile()) {
			QMessageBox::warning(this, tr("Error"), tr("A Scene with this Scene Name exists already in this folder"), QMessageBox::Ok);
		} else {
			if (!rScenePathAbs.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
				QMessageBox::warning(this, tr("Error"), tr("Scene is not inside the Project directory"), QMessageBox::Ok);
			} else {
				if (!GuiContext::getInstance().createNewScene(getSceneName().toStdString(), rScenePathAbs.toStdString(), pathWithFile.toStdString())) {
					QMessageBox::warning(this, tr("Error"), tr("Could not create Scene"), QMessageBox::Ok);
				} else {
					close();
				}
			}
		}
	}

}

void NewSceneDialog::on_cancelButton_clicked()
{
	close();
}

void NewSceneDialog::on_choosePath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select new Scene path"),
												 QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()),
												 QFileDialog::ShowDirsOnly
												 | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		setScenePath(GuiContext::getInstance().toVirtualPath(dir));
	}
}
