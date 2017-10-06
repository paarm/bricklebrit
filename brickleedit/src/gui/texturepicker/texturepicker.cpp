#include "texturepicker.h"
#include "../../project/projectcontext.h"

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
	SelectedItem rSelectedItem;
	rSelectedItem.resourceName=mResourceName;

	if (rNode) {
		if (rNode->getNodeType()==NodeType::Texture) {
			rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNode);
		} else if (rNode->getNodeType()==NodeType::TextureFrame) {
			rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNode->getParent());
			rSelectedItem.rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
		} else if (rNode->getNodeType()==NodeType::Animation) {
			rSelectedItem.rNodeAnimation=static_cast<NodeAnimation*>(rNode);
			if (rSelectedItem.rNodeAnimation->getChildCount()>0) {
				rSelectedItem.rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rSelectedItem.rNodeAnimation->getNodeFromIndex(0));
			}
		} else if (rNode->getNodeType()==NodeType::AnimationFrame) {
			rSelectedItem.rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNode);
			rSelectedItem.rNodeAnimation=static_cast<NodeAnimation*>(rNode->getParent());
		}
		if (rSelectedItem.rNodeAnimationFrame) {
			NodeResource *rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile);
			if (rNodeResource) {
				rSelectedItem.rNodeTexture=static_cast<NodeTexture*>(rNodeResource->getNodeWithNodeId(rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid));
				if (rSelectedItem.rNodeTexture) {
					if (!rSelectedItem.rNodeAnimationFrame->getFrameRef().frame.empty()) {
						rSelectedItem.rNodeTextureFrame=static_cast<NodeTextureFrame*>(rSelectedItem.rNodeTexture->getChildNodeWithNameAndNodeType(rSelectedItem.rNodeAnimationFrame->getFrameRef().frame, NodeType::TextureFrame));
					}
				}
			}
		}
	}
	return rSelectedItem;
}

