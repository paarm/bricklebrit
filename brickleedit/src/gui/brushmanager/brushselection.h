#ifndef BRUSHSELECTION_H
#define BRUSHSELECTION_H

#include <QDialog>
#include <bricklelib.h>
#include "../texturepicker/texturepicker.h"

namespace Ui {
class BrushSelection;
}

class BrushSelection : public QDialog
{
	Q_OBJECT

public:
	explicit BrushSelection(const string& rResourceName, Node *rBrushNode, QWidget *parent = 0);
	~BrushSelection();

private slots:
	void on_resourceNameOpm_activated(const QString &arg1);

	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_cancelButton_clicked();

	void on_BrushSelection_rejected();

	void on_okButton_clicked();

private:
	Ui::BrushSelection  *ui;
	Node				*mNode=nullptr;
	TexturePicker	    *mTexturePicker=nullptr;
	SelectedItem		mSelectedItem;
	void drawPreviewImage();
};

#endif // BRUSHSELECTION_H
