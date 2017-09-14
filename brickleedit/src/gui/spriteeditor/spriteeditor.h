#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>

namespace Ui {
class SpriteEditor;
}

class SpriteEditor : public QDialog
{
	Q_OBJECT

public:
	SpriteEditor(Node *rNode, QWidget *parent = 0);
	~SpriteEditor();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

	void on_SpriteEditor_rejected();

	void on_resourceNameOpm_currentIndexChanged(const QString &arg1);

private:
	Ui::SpriteEditor *ui;
	Node*	mNode=nullptr;
	void setupNode();

};

#endif // SPRITEEDITOR_H
