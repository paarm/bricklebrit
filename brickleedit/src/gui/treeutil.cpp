#include "treeutil.h"
#include "../project/projectcontext.h"
#include <QLabel>
#include <QImage>
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

void TreeUtil::fillTreeWidgetWithTexturesFromResource(QTreeWidget *rQTreeWidget, const string &rResourceName, bool textures, bool animations) {
	rQTreeWidget->clear();
	NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rResourceName);
	if (rNodeResource) {
		int cnt=rNodeResource->getChildCount();
		for (int i=0;i<cnt;i++) {
			Node *rNode=rNodeResource->getNodeFromIndex(i);
			if (rNode && rNode->getNodeType()==NodeType::Texture) {
				NodeTexture* rNodeTexture=static_cast<NodeTexture*>(rNode);
				QTreeWidgetItem *r=new QTreeWidgetItem(rQTreeWidget);
				r->setText(0,QString::fromStdString(TreeUtil::getNodeNameWithId(rNode)));
				//TreeUtil::setTypeNameToTreeItem(r,rNode);
				//TreeUtil::setNodeNameToTreeItem(r,rNode);
				TreeUtil::setNodeDataToTreeItem(r,rNode);

				BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
				if (bTexture) {
					QLabel *rPreviewImage=new QLabel(QString::fromStdString(to_string(i)));
					QImage img(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
					QImage scaled = img.scaled(60, 60, Qt::AspectRatioMode::KeepAspectRatio); // Scale image to show results better
					QPixmap rQPixmap = QPixmap::fromImage(scaled); // Create pixmap from image
					rPreviewImage->setPixmap(rQPixmap);
					rQTreeWidget->setItemWidget(r,1, rPreviewImage);
					//TreeUtil::setNodeToTreeItem(r, rNode);
					rQTreeWidget->addTopLevelItem(r);
					r->setExpanded(true);

					if (rNode->getChildCount()>0) {
						unsigned long countChild=rNode->getChildCount();
						for (unsigned long i=0;i<countChild;i++) {
							Node *rNodeChild=rNode->getNodeFromIndex(i);
							if (rNodeChild && rNodeChild->getNodeType()==NodeType::TextureFrame) {
								NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeChild);
								QTreeWidgetItem *rc=new QTreeWidgetItem(r);
								rc->setText(0,QString::fromStdString("Frame: "+TreeUtil::getNodeNameWithId(rNodeChild)));

								//TreeUtil::setNodeNameToTreeItem(r,rNode);
								TreeUtil::setNodeDataToTreeItem(rc,rNodeChild);

								QLabel *rPreviewImageFrame=new QLabel(QString::fromStdString(rNodeTextureFrame->getName()));

								QImage imgFrame=img.copy(rNodeTextureFrame->getFrame().x,
															 rNodeTextureFrame->getFrame().y,
															 rNodeTextureFrame->getFrame().width,
															 rNodeTextureFrame->getFrame().height);
								QImage scaledFrame = imgFrame.scaled(30, 30, Qt::AspectRatioMode::KeepAspectRatio); // Scale image to show results better
								QPixmap rQPixmapFrame = QPixmap::fromImage(scaledFrame); // Create pixmap from image
								rPreviewImageFrame->setPixmap(rQPixmapFrame);
								rQTreeWidget->setItemWidget(rc,1, rPreviewImageFrame);
								r->addChild(rc);
							}
						}
					}
				}
			}
		}
	}
}

