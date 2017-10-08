#ifndef BRUSHDOCK_H
#define BRUSHDOCK_H

#include <QDockWidget>
#include "brushselection.h"

namespace Ui {
class BrushDock;
}

class BrushDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit BrushDock(QWidget *parent = 0);	
	~BrushDock();
	void setAsBrush(SelectedItem rSelectedItem);
	void clearBrush();
	void setBrushEnabled(bool enabled);
	SelectedItem &getSelectedBrush();
	Node* getSelectedBrushNode();
	void setCurrentPaintCanvas(Node2d *rNode2d);
	int getBrushWidth();
	int getBrushHeight();
	PointFloat getBrushScale();
	PointInt getBrushSize();
	float getRotation();
	NodeSprite* getNodeFromBrush(float worldX, float worldY);
	NodeSprite* getNewNodeFromBrush(float worldX, float worldY);
private slots:
	void on_selectBrush_clicked();

private:
	Ui::BrushDock *ui;
	SelectedItem mSelectedItem;
	Node2d		*mCurrentPaintCanvas=nullptr;
	NodeSprite	mNodeFromBrush;
};

#endif // BRUSHDOCK_H
