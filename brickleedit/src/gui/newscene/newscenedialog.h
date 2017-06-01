#ifndef NEWSCENEDIALOG_H
#define NEWSCENEDIALOG_H

#include <QDialog>
#include "../../project/nodeinfotype.h"

namespace Ui {
class NewSceneDialog;
}

class NewSceneDialog : public QDialog
{
private:
	NodeInfoType		mNodeInfoType;
	Ui::NewSceneDialog *ui;
	Q_OBJECT

public:
	explicit NewSceneDialog(NodeInfoType rNodeInfoType, QWidget *parent = 0);
	~NewSceneDialog();
	void setName(const QString& rSceneName);
	QString getName();
	void setScenePath(const QString& rScenePath);
	QString getScenePath();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

	void on_choosePath_clicked();
};

#endif // NEWSCENEDIALOG_H
