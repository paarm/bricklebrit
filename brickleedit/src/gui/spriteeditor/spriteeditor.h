#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>

namespace Ui {
class SpriteEditor;
}

struct CurrentRef {
	//Node	*rNode;
	string	resourceName;
	int		id;
	string	frameName;
};

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

	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::SpriteEditor *ui;
	NodeSprite*	mNode=nullptr;
	CurrentRef mCurrentRef;
	void setupNode();
	void updateSelectedFrame(Node *rNode, bool setWidthAndHeight);


};

#endif // SPRITEEDITOR_H
