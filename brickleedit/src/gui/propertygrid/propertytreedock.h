#pragma once

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <bricklelib.h>
#include <QLineEdit>
#include <QItemDelegate>
#include "propertyinfo.h"
#include "propertyeditdelegate.h"
#include "../../project/nodeinfotype.h"

namespace Ui {
class PropertyTreeDock;
}

class PropertyTreeDock : public QDockWidget, public IProperty
{
	Q_OBJECT

public:
	explicit PropertyTreeDock(QWidget *parent = 0);
	~PropertyTreeDock();
	void clear();
	void setPropertiesForNode(Node* rNode, NodeInfoType rNodeInfoType);
	PropertyInfo getPropertyInfoFromTreeItem(QTreeWidgetItem* r);
	void setPropertyInfoToTreeItem(QTreeWidgetItem* r, PropertyInfo &rPropertyInfo);
	void onChangeProperty(PropertyInfo rPropertyInfo);
	virtual bool onPropertyChange(PropertyInfo* rPropertyInfo, QString data) override;
private slots:
	void on_propertyTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_propertyTree_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::PropertyTreeDock *ui;
	NodeInfoType mNodeInfoType;
	PropertyEditDelegate *mPropertyNumberDelegate=nullptr;

	void addSubProperty(Node* rNode, PropertyBase* rPropertyBase, const string& propertyName, QTreeWidgetItem* r, string rValue, const string &rPropertySubName);

	QString propertyToString(PropertyBase* p);
	bool validatePropertyInt(QString data);
};
