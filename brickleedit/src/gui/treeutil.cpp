#include "treeutil.h"
#include "../project/projectcontext.h"
#include <QLabel>
#include <QPixmap>

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

void TreeUtil::setPixmapToTreeItem(QTreeWidget *rQTreeWidget, QTreeWidgetItem *r, QImage& rImageSrc, int srcX, int srcY, int srcWidht, int srcHeight, int destWidth, int destHeight) {
	QLabel *rPreviewImage=new QLabel("");
	QImage imgFrame=rImageSrc.copy(srcX,
								 srcY,
								 srcWidht,
								 srcHeight);
	QImage scaledFrame = imgFrame.scaled(destWidth, destHeight, Qt::AspectRatioMode::KeepAspectRatio); // Scale image to show results better
	QPixmap rQPixmapFrame = QPixmap::fromImage(scaledFrame); // Create pixmap from image
	rPreviewImage->setPixmap(rQPixmapFrame);
	rQTreeWidget->setItemWidget(r,1, rPreviewImage);
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

				BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
				if (bTexture) {
					QImage rImageSrc(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
					TreeUtil::setPixmapToTreeItem(rQTreeWidget, r, rImageSrc, 0, 0, bTexture->width, bTexture->height, 60, 60);
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
								TreeUtil::setPixmapToTreeItem(rQTreeWidget, rc, rImageSrc, rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y, rNodeTextureFrame->getFrame().width, rNodeTextureFrame->getFrame().height, 30, 30);
								r->addChild(rc);
							}
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
									BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
									if (bTexture) {
										NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithNameAndNodeType(rNodeAnimationFrame->getFrameRef().frame, NodeType::TextureFrame));
										if (rNodeTextureFrame && rNodeTextureFrame->getNodeType()==NodeType::TextureFrame) {
											QImage rImageSrc(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
											QTreeWidgetItem *rc=new QTreeWidgetItem(r);
											rc->setText(0,QString::fromStdString("Frame: "+TreeUtil::getNodeNameWithId(rNodeAnimationFrame)));
											TreeUtil::setNodeDataToTreeItem(rc,rNodeAnimationFrame);
											TreeUtil::setPixmapToTreeItem(rQTreeWidget, rc, rImageSrc, rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y, rNodeTextureFrame->getFrame().width, rNodeTextureFrame->getFrame().height, 30, 30);
											r->addChild(rc);
											if (isFirst) {
												isFirst=false;
												TreeUtil::setPixmapToTreeItem(rQTreeWidget, r, rImageSrc, rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y, rNodeTextureFrame->getFrame().width, rNodeTextureFrame->getFrame().height, 30, 30);
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
	}
}

