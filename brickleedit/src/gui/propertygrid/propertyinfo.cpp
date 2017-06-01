#include "propertyinfo.h"

PropertyInfo::PropertyInfo() {}

PropertyInfo::PropertyInfo(Node *rNode, PropertyBase* rPropertyBase, const string& rPropertyName, const string& rPropertySubName, QTreeWidgetItem* rTreeWidgetItem, QTreeWidgetItem* rTreeWidgetSubItem) {
	mNode=rNode;
	mPropertyBase=rPropertyBase;
	mPropertyName=rPropertyName;
	mPropertySubName=rPropertySubName;
	mTreeWidgetItem=rTreeWidgetItem;
	mTreeWidgetSubItem=rTreeWidgetSubItem;
}

PropertyBase* PropertyInfo::getPropertyBase() {
	return mPropertyBase;
}

const string& PropertyInfo::getPropertyName() {
	return mPropertyName;
}
const string& PropertyInfo::getPropertySubName() {
	return mPropertySubName;
}
Node* PropertyInfo::getNode() {
	return mNode;
}

QTreeWidgetItem *PropertyInfo::getTreeWidgetItem() {
	return mTreeWidgetItem;
}

QTreeWidgetItem *PropertyInfo::getTreeWidgetSubItem() {
	return mTreeWidgetSubItem;
}

