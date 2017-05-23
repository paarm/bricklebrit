#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QDockWidget>
#include <bricklelib.h>

namespace Ui {
class PropertyTreeDock;
}

class PropertyTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit PropertyTreeDock(QWidget *parent = 0);
	~PropertyTreeDock();

	void clear();
	void setPropertiesForNode(Node* rNode);
private:
	Ui::PropertyTreeDock *ui;
};

#endif // PROPERTYWIDGET_H
