#pragma once
#include <bricklelib.h>
#include <QMetaType>
#include <QTreeWidgetItem>

class PropertyInfo {
	Node*	mNode=nullptr;
	PropertyBase* mPropertyBase=nullptr;
	string  mPropertyName;
	string  mPropertySubName;
	QTreeWidgetItem *mTreeWidgetItem=nullptr;
	QTreeWidgetItem *mTreeWidgetSubItem=nullptr;
public:
	PropertyInfo();
	PropertyInfo(Node *rNode, PropertyBase* rPropertyBase, const string& rPropertyName, const string& rPropertySubName, QTreeWidgetItem* rTreeWidgetItem, QTreeWidgetItem* rTreeWidgetSubItem);

	Node* getNode();
	PropertyBase* getPropertyBase();
	const string& getPropertyName();
	const string& getPropertySubName();
	QTreeWidgetItem *getTreeWidgetItem();
	QTreeWidgetItem *getTreeWidgetSubItem();
};
Q_DECLARE_METATYPE(PropertyInfo);

