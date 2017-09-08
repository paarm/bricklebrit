#ifndef NEWSCENENODEDIALOG_H
#define NEWSCENENODEDIALOG_H

#include <QDialog>
#include "../../../project/nodeinfotype.h"


namespace Ui {
class NewSceneNodeDialog;
}

class NewSceneNodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewSceneNodeDialog(NodeInfoType rNodeInfoType, QWidget *parent = 0);
	~NewSceneNodeDialog();

private slots:
	void on_listWidget_doubleClicked(const QModelIndex &index);
	void on_cancelButton_clicked();

signals:
	void newNodeSelected(QString rNodeTypeName, NodeInfoType rNodeInfoType);
private:
	Ui::NewSceneNodeDialog *ui;
	NodeInfoType		mNodeInfoType=NodeInfoType::Scene;
};

#endif // NEWSCENENODEDIALOG_H
