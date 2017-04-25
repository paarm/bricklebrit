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

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);

private:
	Ui::NewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_H
