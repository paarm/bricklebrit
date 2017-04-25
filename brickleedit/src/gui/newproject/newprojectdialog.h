#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewProjectDialog(QWidget *parent = 0);
	~NewProjectDialog();
	void init();
	void setProjectName(const QString& rProjectName);
	QString getProjectName();
	void setProjectPath(const QString& rProjectPath);
	QString getProjectPath();

private slots:
	void on_choosePath_clicked();
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::NewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_H
