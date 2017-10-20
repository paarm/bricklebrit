#include "treeutil.h"
#include "../project/projectcontext.h"
#include <QLabel>
#include "previewimageutil.h"

Node* TreeUtil::getNodeFromTreeItem(QTreeWidgetItem* r) {
	Node *rv=nullptr;
	if (r) {
		QVariant v=r->data(0,Qt::UserRole);
		rv=v.value<Node*>();
	}
	return rv;
}

void TreeUtil::setNodeDataToTreeItem(QTreeWidgetItem* r, Node *rNode) {
	if (r && rNode) {
		QVariant v;
		v.setValue(rNode);
		r->setData(0, Qt::UserRole, v);
	}
}

void TreeUtil::setTypeNameToTreeItem(QTreeWidgetItem* r, Node *rNode) {
	if (r && rNode) {
		string rType=getNodeTypeAsString(rNode->getNodeType());
		rType+="("+std::to_string(rNode->getId())+")";
		r->setText(0, QString::fromStdString(rType));
	}
}

string TreeUtil::getNodeNameWithId(Node *rNode) {
	string rv=rNode->getName();
	rv+="("+std::to_string(rNode->getId())+")";
	return rv;
}

void TreeUtil::setNodeNameToTreeItem(QTreeWidgetItem* r, Node* rNode) {
	if (r && rNode) {
		r->setText(1,QString::fromStdString(rNode->getName()));
	}
}

void TreeUtil::setNodeToTreeItem(QTreeWidgetItem* r, Node* rNode) {
	if (r && rNode) {
		TreeUtil::setTypeNameToTreeItem(r,rNode);
		TreeUtil::setNodeNameToTreeItem(r,rNode);
		TreeUtil::setNodeDataToTreeItem(r,rNode);
	}
}


QTreeWidgetItem *TreeUtil::getTreeWidgetItemFromNode_Parent(QTreeWidget *rQTreeWidget, QTreeWidgetItem* rParent, Node *rNode) {
	QTreeWidgetItem *rv=nullptr;
	int cnt=rParent->childCount();
	for (int i=0;i<cnt && !rv;i++) {
		QTreeWidgetItem *r=rParent->child(i);
		if (r) {
			Node *rNodeCurrent=TreeUtil::getNodeFromTreeItem(r);
			if (rNodeCurrent && rNodeCurrent->getId()==rNode->getId()) {
				rv=r;
				break;
			}
			rv=TreeUtil::getTreeWidgetItemFromNode_Parent(rQTreeWidget, r, rNode);
		}
	}
	return rv;
}

QTreeWidgetItem *TreeUtil::getTreeWidgetItemFromNode(QTreeWidget *rQTreeWidget, Node *rNode) {
	QTreeWidgetItem *rv=nullptr;
	int cnt=rQTreeWidget->topLevelItemCount();
	for (int i=0;i<cnt && !rv;i++) {
		QTreeWidgetItem *r=rQTreeWidget->topLevelItem(i);
		Node *rNodeCurrent=TreeUtil::getNodeFromTreeItem(r);
		if (rNodeCurrent && rNodeCurrent->getId()==rNode->getId()) {
			rv=r;
			break;
		}
		rv=TreeUtil::getTreeWidgetItemFromNode_Parent(rQTreeWidget, r, rNode);
	}
	return rv;
}

void TreeUtil::fillTreeWidgetWithTexturesFromResource(QTreeWidget *rQTreeWidget, const string &rResourceName, bool addTextures, bool addAnimations) {
	rQTreeWidget->clear();
	NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rResourceName);
	if (rNodeResource) {
		int cnt=rNodeResource->getChildCount();
		QIcon rTextureIcon(":/icons/newTexture.png");
		QIcon rAnimationIcon(":/icons/newAnimation.png");
		for (int i=0;i<cnt;i++) {
			Node *rNode=rNodeResource->getNodeFromIndex(i);
			if (rNode && rNode->getNodeType()==NodeType::Texture && addTextures) {
				NodeTexture* rNodeTexture=static_cast<NodeTexture*>(rNode);
				QTreeWidgetItem *r=new QTreeWidgetItem(rQTreeWidget);
				r->setText(0,QString::fromStdString(TreeUtil::getNodeNameWithId(rNode)));
				TreeUtil::setNodeDataToTreeItem(r,rNode);
				r->setIcon(0, rTextureIcon);
				r->setIcon(1, QIcon(PreviewImageUtil::getPreviewImage(rNodeTexture, nullptr, 60, 60)));
				rQTreeWidget->addTopLevelItem(r);
				//r->setExpanded(true);

				if (rNode->getChildCount()>0) {
					unsigned long countChild=rNode->getChildCount();
					for (unsigned long i=0;i<countChild;i++) {
						Node *rNodeChild=rNode->getNodeFromIndex(i);
						if (rNodeChild && rNodeChild->getNodeType()==NodeType::TextureFrame) {
							NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeChild);
							QTreeWidgetItem *rc=new QTreeWidgetItem(r);
							rc->setText(0,QString::fromStdString("Frame: "+TreeUtil::getNodeNameWithId(rNodeChild)));
							TreeUtil::setNodeDataToTreeItem(rc,rNodeChild);
							rc->setIcon(1, QIcon(PreviewImageUtil::getPreviewImage(rNodeTexture, rNodeTextureFrame, 30, 30)));
							r->addChild(rc);
						}
					}
				}
			} else if (rNode && rNode->getNodeType()==NodeType::Animation && addAnimations) {
				NodeAnimation* rNodeAnimation=static_cast<NodeAnimation*>(rNode);
				QTreeWidgetItem *r=new QTreeWidgetItem(rQTreeWidget);
				r->setText(0,QString::fromStdString(TreeUtil::getNodeNameWithId(rNodeAnimation)));
				TreeUtil::setNodeDataToTreeItem(r,rNodeAnimation);
				r->setIcon(0, rAnimationIcon);

				if (rNodeAnimation->getChildCount()>0) {
					unsigned long countChild=rNodeAnimation->getChildCount();
					bool isFirst=true;
					for (unsigned long i=0;i<countChild;i++) {
						Node *rNodeChild=rNodeAnimation->getNodeFromIndex(i);
						if (rNodeChild && rNodeChild->getNodeType()==NodeType::AnimationFrame) {
							NodeAnimationFrame *rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNodeChild);
							NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rNodeAnimationFrame->getFrameRef().resourcefile);
							if (rNodeResource) {
								NodeTexture* rNodeTexture=static_cast<NodeTexture*>(rNodeResource->getNodeWithNodeId(rNodeAnimationFrame->getFrameRef().textureid));
								if (rNodeTexture && rNodeTexture->getNodeType()==NodeType::Texture) {
									NodeTextureFrame *rNodeTextureFrame=nullptr;
									if (!rNodeAnimationFrame->getFrameRef().frame.empty()) {
										rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithNameAndNodeType(rNodeAnimationFrame->getFrameRef().frame, NodeType::TextureFrame));
									}

									QTreeWidgetItem *rc=new QTreeWidgetItem(r);
									QPixmap rPixmap=PreviewImageUtil::getPreviewImage(rNodeTexture, rNodeTextureFrame, 30, 30);
									rc->setIcon(1, rPixmap);
									rc->setText(0,QString::fromStdString("Frame: "+TreeUtil::getNodeNameWithId(rNodeAnimationFrame)));
									TreeUtil::setNodeDataToTreeItem(rc,rNodeAnimationFrame);

									r->addChild(rc);
									if (isFirst) {
										isFirst=false;
										r->setIcon(1, rPixmap);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

