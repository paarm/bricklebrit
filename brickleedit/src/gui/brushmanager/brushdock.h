#ifndef BRUSHDOCK_H
#define BRUSHDOCK_H

#include <QDockWidget>
#include "brushselection.h"

namespace Ui {
class BrushDock;
}

struct GridState {
    bool isActive;
    int gridX;
    int gridY;
    int offsetX;
    int offsetY;
};

class BrushDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit BrushDock(QWidget *parent = 0);	
	~BrushDock();
    void setAsBrush(SelectedItem rSelectedItem, SelectedItemPref *rSelectedItemPref);
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
    GridState getGridState();
private slots:
	void on_selectBrush_clicked();
    void on_gridActive_clicked(bool checked);

private:
	Ui::BrushDock *ui;
	SelectedItem mSelectedItem;
	Node2d		*mCurrentPaintCanvas=nullptr;
	NodeSprite	mNodeFromBrush;
    void setGridFieldsState();
    int calcGridPos(int worldPos, int gridSize, int gridOffset);


};

#endif // BRUSHDOCK_H
