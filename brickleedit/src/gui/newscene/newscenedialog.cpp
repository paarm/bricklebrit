#include "newscenedialog.h"
#include "ui_newscenedialog.h"
#include "../guicontext.h"

NewSceneDialog::NewSceneDialog(NodeInfoType rNodeInfoType, QWidget *parent) :
	QDialog(parent),
	mNodeInfoType(rNodeInfoType),
	ui(new Ui::NewSceneDialog)
{
	ui->setupUi(this);
}

NewSceneDialog::~NewSceneDialog()
{
	delete ui;
}

void NewSceneDialog::setName(const QString& rSceneName) {
	ui->name->setText(rSceneName);
}

QString NewSceneDialog::getName() {
	return ui->name->text().trimmed();
}

void NewSceneDialog::setScenePath(const QString& rScenePath) {
	ui->path->setText(rScenePath);
}

QString NewSceneDialog::getScenePath() {
	return ui->path->text().trimmed();
}

void NewSceneDialog::on_okButton_clicked()
{
	QString rScenePathAbs=GuiContext::getInstance().fromVirtualPath(getScenePath());

	if (getName().toStdString().size()==0 || rScenePathAbs.size()==0) {
		QMessageBox::warning(this, tr("Error"), tr("Name and Path are required"), QMessageBox::Ok);
	} else {
		QString pathWithFile;
		if (mNodeInfoType==NodeInfoType::Scene) {
			pathWithFile = QDir(rScenePathAbs).filePath(getName()+".brscn");
		} else {
			pathWithFile = QDir(rScenePathAbs).filePath(getName()+".brres");
		}
		QFileInfo check_file(pathWithFile);
		// check if file exists and if yes: Is it really a file and no directory?
		if (check_file.exists() && check_file.isFile()) {
			if (mNodeInfoType==NodeInfoType::Scene) {
				QMessageBox::warning(this, tr("Error"), tr("A Scene with this Name exists already in this folder"), QMessageBox::Ok);
			} else {
				QMessageBox::warning(this, tr("Error"), tr("A Resource with this Name exists already in this folder"), QMessageBox::Ok);
			}
		} else {
			if (!rScenePathAbs.startsWith(QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()))) {
				if (mNodeInfoType==NodeInfoType::Scene) {
					QMessageBox::warning(this, tr("Error"), tr("Scene is not inside the Project directory"), QMessageBox::Ok);
				} else {
					QMessageBox::warning(this, tr("Error"), tr("Resource is not inside the Project directory"), QMessageBox::Ok);
				}
			} else {
				if (mNodeInfoType==NodeInfoType::Scene) {
					if (!GuiContext::getInstance().createNewScene(getName().toStdString(), rScenePathAbs.toStdString(), pathWithFile.toStdString())) {
						QMessageBox::warning(this, tr("Error"), tr("Could not create Scene"), QMessageBox::Ok);
					} else {
						close();
					}
				} else {
					if (!GuiContext::getInstance().createNewResource(getName().toStdString(), rScenePathAbs.toStdString(), pathWithFile.toStdString())) {
						QMessageBox::warning(this, tr("Error"), tr("Could not create Resource"), QMessageBox::Ok);
					} else {
						close();
					}
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
												 GuiContext::getInstance().fromVirtualPath(getScenePath()),
												 QFileDialog::ShowDirsOnly
												 | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()) {
		setScenePath(GuiContext::getInstance().toVirtualPath(dir));
	}
}
