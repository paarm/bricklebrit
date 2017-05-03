#ifndef NEWSCENENODEDIALOG_H
#define NEWSCENENODEDIALOG_H

#include <QDialog>

namespace Ui {
class NewSceneNodeDialog;
}

class NewSceneNodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewSceneNodeDialog(QWidget *parent = 0);
	~NewSceneNodeDialog();

private slots:
	void on_listWidget_doubleClicked(const QModelIndex &index);
signals:
	void newNodeSelected(QString rNodeTypeName);
private:
	Ui::NewSceneNodeDialog *ui;
};

#endif // NEWSCENENODEDIALOG_H
