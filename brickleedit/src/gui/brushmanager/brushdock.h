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
private slots:
	void on_selectBrush_clicked();

private:
	Ui::BrushDock *ui;
	SelectedItem mSelectedItem;
};

#endif // BRUSHDOCK_H
