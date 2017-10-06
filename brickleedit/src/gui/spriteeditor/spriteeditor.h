#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>
#include "../texturepicker/texturepicker.h"

namespace Ui {
class SpriteEditor;
}

struct CurrentRef {
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

	void on_resourceNameOpm_activated(const QString &arg1);

private:
	Ui::SpriteEditor	*ui;
	TexturePicker		*mTexturePicker=nullptr;
	SelectedItem		mSelectedItem;
	NodeSprite*			mNode=nullptr;

	void drawPreviewImage(bool setWidthAndHeight);
};

#endif // SPRITEEDITOR_H
