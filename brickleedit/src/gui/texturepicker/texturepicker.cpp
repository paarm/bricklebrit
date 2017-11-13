#include "texturepicker.h"
#include "../guicontext.h"

TexturePicker::TexturePicker(QTreeWidget *rTreeWidget)
 : mTreeWidget(rTreeWidget) {

}

SelectedItem TexturePicker::initialize(const string &rResourceName, Node *rNodeToInit) {
	SelectedItem rSelectedItem;

	switchToResource(rResourceName);
	if (rNodeToInit) {
		QTreeWidgetItem *r=TreeUtil::getTreeWidgetItemFromNode(mTreeWidget, rNodeToInit);
		if (r) {
			r->setSelected(true);
			if (r->parent()) {
				r->parent()->setExpanded(true);
			}
			mTreeWidget->scrollToItem(r);
			rSelectedItem=setSelectedNode(rNodeToInit);
		}
	}
	return rSelectedItem;
}

void TexturePicker::switchToResource(const string&rResourceName)
{
	if (mResourceName!=rResourceName) {
		mResourceName=rResourceName;
		TreeUtil::fillTreeWidgetWithTexturesFromResource(mTreeWidget, rResourceName, true, true);
	}
}

SelectedItem TexturePicker::setSelectedNode(Node* rNode) {
    return GuiContext::getInstance().prepareSelectedNodeFromTextureOrAnimationNode(rNode, mResourceName);
}

