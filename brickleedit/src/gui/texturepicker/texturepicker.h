#ifndef TEXTUREPICKER_H
#define TEXTUREPICKER_H
#include <bricklelib.h>
#include <QComboBox>
#include <QTreeWidget>
#include <QLabel>
#include "../treeutil.h"
#include "selecteditem.h"
class TexturePicker
{
private:
	string		mResourceName;
	QTreeWidget *mTreeWidget;
	void selectedNodeChanged(Node* rNode);
public:
	TexturePicker(QTreeWidget *rTreeWidget);
	SelectedItem initialize(const string& rResourceName, Node* rNode);
	SelectedItem setSelectedNode(Node* rNode);
	void switchToResource(const string&rResourceName);
};

#endif // TEXTUREPICKER_H
