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
	void setCurrentPaintCanvas(Node2d *rNode2d);
	Node2d* getCurrentPaintCanvas();
	void setCurrentSelectionAsBrush();
	void setSelectedItemAsBrush(SelectedItem rSelectedItem);

	void updateBrushNodesPosition(float worldX, float worldY);
	void commitBrushNodes();
	void clearBrush();
	void setBrushPossible(bool enabled);
	void BrushToolActivated(bool activated);
private slots:
	void on_selectBrush_clicked();

	void on_flipX_toggled(bool checked);

	void on_flipY_toggled(bool checked);

	void on_rotate_valueChanged(int arg1);

	void on_scaleX_valueChanged(double arg1);

	void on_scaleY_valueChanged(double arg1);

	void on_sizeX_valueChanged(int arg1);

	void on_sizeY_valueChanged(int arg1);

private:
	void createNewBrushNodes();
	void removeBrushNodes();
	void setCenterBrushUIInfo(BrushInfoItem& rBrushInfoItem);
	Node* getSelectedBrushNode();
	void setNodesAsBrush(vector<Node*> &v);

	Ui::BrushDock *ui;
	Node2d				*mCurrentPaintCanvas=nullptr;
	vector<NodeSprite*> mCurrentBrushNodes;


	vector<BrushInfoItem> mBrushInfoItemList;
	size_t				  mBrushInfoCenterIndex=0;
};

#endif // BRUSHDOCK_H
