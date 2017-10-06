#ifndef NEWSCENEDIALOG_H
#define NEWSCENEDIALOG_H

#include <QDialog>

namespace Ui {
class NewSceneDialog;
}

class NewSceneDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::NewSceneDialog *ui;
	bool mIsResource;
public:
	explicit NewSceneDialog(bool isResource, QWidget *parent = 0);
	~NewSceneDialog();
	void setName(QString rName);
	QString getName();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

};

#endif // NEWSCENEDIALOG_H
