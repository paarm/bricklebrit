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
	void setCurrentSelectionAsBrush();
	void setNodesAsBrush(vector<Node*> v);
	void setSelectedItemAsBrush(SelectedItem rSelectedItem);


	void clearBrush();
	void setBrushEnabled(bool enabled);
	Node* getSelectedBrushNode();
	vector<NodeSprite*>& getNodesFromBrush(float worldX, float worldY);
	vector<NodeSprite*> getNewNodesFromBrush(float worldX, float worldY);
private slots:
	void on_selectBrush_clicked();

private:
	Ui::BrushDock *ui;
	Node2d		*mCurrentPaintCanvas=nullptr;
	vector<NodeSprite*>	mNodeFromBrushList;
	vector<NodeSprite*>	mNodeFromBrushListCache;

	vector<BrushInfoItem> mBrushInfoItemList;
	size_t				  mBrushInfoCenterIndex=0;
	void setCenterBrushUIInfo(BrushInfoItem& rBrushInfoItem);
	bool				mBrushIsInvalid=true;
};

#endif // BRUSHDOCK_H
