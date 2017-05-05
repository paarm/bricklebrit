#ifndef NEWSCENEDIALOG_H
#define NEWSCENEDIALOG_H

#include <QDialog>

namespace Ui {
class NewSceneDialog;
}

class NewSceneDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewSceneDialog(QWidget *parent = 0);
	~NewSceneDialog();
	void setSceneName(const QString& rSceneName);
	QString getSceneName();
	void setScenePath(const QString& rScenePath);
	QString getScenePath();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

	void on_choosePath_clicked();

private:
	Ui::NewSceneDialog *ui;
};

#endif // NEWSCENEDIALOG_H
