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
	void setName(QString rName);
	QString getName();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

};

#endif // NEWSCENEDIALOG_H
