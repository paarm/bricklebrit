#ifndef SCENETREEDOCK_H
#define SCENETREEDOCK_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <bricklelib.h>
#include "../treeutil.h"
#include <QTranslator>
#include <QPushButton>
#include "../textureframeeditor/textureframeeditor.h"
#include "../animationframeeditor/animationframeeditor.h"
#include "../spriteeditor/spriteeditor.h"
namespace Ui {
class SceneTreeDock;
}

class TabInfo : QObject {
public:
	//TabInfo(QWidget *rParentWidget) : mParentWidget(rParentWidget) {}
	void setQTreeWidget(QTreeWidget *rQTreeWidget) {
		mTreeWidget=rQTreeWidget;
	}
	QTreeWidgetItem* getSelectedItem() {
		QTreeWidgetItem* rv=nullptr;
		QList<QTreeWidgetItem*>selectedItems=mTreeWidget->selectedItems();
		if (!selectedItems.isEmpty()) {
			rv=selectedItems[0];
		}
		return rv;
	}
	Node* getSelectedNode() {
		QTreeWidgetItem*r=getSelectedItem();
		Node *rv=nullptr;
		if (r) {
			rv=TreeUtil::getNodeFromTreeItem(r);
		}
		return rv;
	}
	QTreeWidgetItem* getRootItem() {
		QTreeWidgetItem* rv=nullptr;
		if (mTreeWidget->topLevelItemCount()>0) {
			rv=mTreeWidget->topLevelItem(0);
		}
		return rv;
	}
	Node* getRootNode() {
		QTreeWidgetItem*r=getRootItem();
		Node *rv=nullptr;
		if (r) {
			rv=TreeUtil::getNodeFromTreeItem(r);
		}
		return rv;
	}

	void deleteChilds(QTreeWidgetItem *parent) {
		if (parent) {
			int cnt=parent->childCount();
			for (int i=cnt-1;i>=0;i--) {
				QTreeWidgetItem *child=parent->child(i);
				if (child) {
					parent->removeChild(child);
					delete child;
				}
			}
		}
	}

    void deleteItemAndItsChilds(QTreeWidgetItem *toDelete) {
        if (toDelete) {
            if (toDelete->childCount()>0) {
                int cnt=toDelete->childCount();
                for (int i=cnt-1;i>=0;i--) {
                    QTreeWidgetItem *child=toDelete->child(i);
                    deleteItemAndItsChilds(child);
                }
            }
            toDelete->parent()->removeChild(toDelete);
            delete toDelete;
        }
    }

	void updateChildNodes(Node* rNode) {
		int count=mTreeWidget->topLevelItemCount();
		QTreeWidgetItem* rQTreeWidgetItem_found=nullptr;
		for (int i=0;i<count && !rQTreeWidgetItem_found;i++) {
			QTreeWidgetItem* p=mTreeWidget->topLevelItem(i);

			rQTreeWidgetItem_found=searchTreeWidgetItemByNode(p,rNode);
		}
		if (rQTreeWidgetItem_found) {
			deleteChilds(rQTreeWidgetItem_found);

			int childNodes=rNode->getChildCount();
			for (int i=0;i<childNodes;i++) {
				Node *rNodeChild=rNode->getNodeFromIndex(i);
				if (rNodeChild) {
					addNode(rQTreeWidgetItem_found, rNodeChild, false);
				}
			}
		}
	}
	void updateNodeName(Node* rUpdateNode) {
		updateNodeNameX(mTreeWidget->topLevelItem(0), rUpdateNode);
	}

	void clear() {
		mTreeWidget->clear();
	}

	void setItemAsSelectedFromNode(Node* rNode) {
		QTreeWidgetItem* item=searchTreeWidgetItemByNode(getRootItem(), rNode);
		if (item) {
			auto selectedItems=mTreeWidget->selectedItems();
			for(QTreeWidgetItem* r: selectedItems) {
				r->setSelected(false);
				if (r->parent() && r->parent()->isExpanded()) {
					Node *rNode=TreeUtil::getNodeFromTreeItem(r->parent());
					if (rNode && rNode->getNodeType()!=NodeType::Scene && rNode->getNodeType()!=NodeType::Resource) {
						r->parent()->setExpanded(false);
					}
				}
			}
			//mTreeWidget->clearSelection();
			item->setSelected(true);
			if (item->parent() && !item->parent()->isExpanded()) {
				item->parent()->setExpanded(true);
			}
			mTreeWidget->scrollToItem(item);
		}
	}

	QTreeWidgetItem* addNode(QTreeWidgetItem *parent, Node* rNode, bool expandTree) {
		QTreeWidgetItem* r=nullptr;
		bool			addChilds=true;
		if (rNode) {
			if (parent) {
				r=new QTreeWidgetItem(parent);
			} else {
				r=new QTreeWidgetItem(mTreeWidget);
			}
			// Type + ID
			TreeUtil::setTypeNameToTreeItem(0, r,rNode);
			// Name
			TreeUtil::setNodeNameToTreeItem(1, r, rNode);
			//r->setText(1,QString::fromStdString(rNode->getName()));
			TreeUtil::setNodeDataToTreeItem(r,rNode);
			//r->setIcon(0, QIcon(":/icons/new.png"));
			if (parent) {
				parent->addChild(r);
				if (expandTree) {
					//parent->setExpanded(true);
				}
			} else {
				mTreeWidget->addTopLevelItem(r);
				//r->setExpanded(true);
			}
			if (rNode->getNodeType()==NodeType::Texture) {
				QPushButton *rButton=new QPushButton("T",mTreeWidget);
				rButton->setToolTip(QObject::tr("Edit Frames..."));
				rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
				connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
					TextureFrameEditor *rTextureFrameEditor=new TextureFrameEditor(rNode, mTreeWidget);
					rTextureFrameEditor->show();
				});
				mTreeWidget->setItemWidget(r, 2, rButton);
				addChilds=false;
			} else if (rNode->getNodeType()==NodeType::Animation) {
				QPushButton *rButton=new QPushButton("A",mTreeWidget);
				rButton->setToolTip(QObject::tr("Edit Animation Frames..."));
				rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
				connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
					AnimationFrameEditor *rAnimationFrameEditor=new AnimationFrameEditor(rNode, mTreeWidget);
					rAnimationFrameEditor->show();
				});
				mTreeWidget->setItemWidget(r, 2, rButton);
				addChilds=false;
			} else if (rNode->getNodeType()==NodeType::Sprite) {
				QPushButton *rButton=new QPushButton("S",mTreeWidget);
				rButton->setToolTip(QObject::tr("Edit Sprite..."));
				rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
				connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
					SpriteEditor *rSpriteEditor=new SpriteEditor(rNode, mTreeWidget);
					rSpriteEditor->show();
				});
				mTreeWidget->setItemWidget(r, 2, rButton);
			}

			if (addChilds && rNode->getChildCount()>0) {
				unsigned long count=rNode->getChildCount();
				for (unsigned long i=0;i<count;i++) {
					Node *rNodeChild=rNode->getNodeFromIndex(i);
					if (rNodeChild) {
						addNode(r, rNodeChild, expandTree);
					}
				}
			}
		}
		return r;
	}

	QTreeWidgetItem* searchTreeWidgetItemByNode(Node* rNode) {
		int count=mTreeWidget->topLevelItemCount();
		QTreeWidgetItem* rQTreeWidgetItem_found=nullptr;
		for (int i=0;i<count && !rQTreeWidgetItem_found;i++) {
			QTreeWidgetItem* p=mTreeWidget->topLevelItem(i);

			rQTreeWidgetItem_found=searchTreeWidgetItemByNode(p,rNode);
		}
		return rQTreeWidgetItem_found;
	}


private:
	QTreeWidget * mTreeWidget;

	QTreeWidgetItem* searchTreeWidgetItemByNode(QTreeWidgetItem *parent, Node* rNode) {
		QTreeWidgetItem* rv=nullptr;

		if (parent && TreeUtil::getNodeFromTreeItem(parent)==rNode) {
			rv=parent;
		}
		if (!rv) {
			int count=parent->childCount();
			for (int i=0;i<count;i++) {
				QTreeWidgetItem *r=parent->child(i);
				if (r && TreeUtil::getNodeFromTreeItem(r)==rNode) {
					rv=r;
				}
			}
		}
		if (!rv) {
			int count=parent->childCount();
			for (int i=0;i<count && !rv;i++) {
				QTreeWidgetItem *r=parent->child(i);
				rv=searchTreeWidgetItemByNode(r, rNode);
			}
		}
		return rv;
	}

	void setNodeName(QTreeWidgetItem *item, const string& rName) {
		if (item) {
			item->setText(1,QString::fromStdString(rName));
		}
	}

	bool updateNodeNameX(QTreeWidgetItem *parent, Node* rUpdateNode) {
		bool updated=false;
		if (parent) {
			Node* rNode=TreeUtil::getNodeFromTreeItem(parent);
			if (rNode && rNode==rUpdateNode) {
				setNodeName(parent, rNode->getName());
				updated=true;
			}
			if (!updated) {
				int count=parent->childCount();
				for (int i=0;i<count && !updated;i++) {
					QTreeWidgetItem *r=parent->child(i);
					updated=updateNodeNameX(r, rUpdateNode);
				}
			}
		}
		return updated;
	}
};

class SceneTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit SceneTreeDock(QWidget *parent = 0);
	~SceneTreeDock();

	void removeAllLayer();
	void addLayerNode(NodeLayer *rNode);
	void setCurrentLayerAsSelected();
	void updateLayerName(NodeLayer *rNode);


	Node* getSelectedSceneNode();
	Node* getSelectedResourceNode();
	Node* getRootSceneNode();
	Node* getRootResourceNode();
	void updateChildNodes(Node* rNode);
	void updateNodeName(Node* rUpdateNode);
	//void switchToScene(Node *rNode);
	void switchToResource(Node *rNode);
	void addSceneNodeToSelectedItem(Node *rNode);
	void addSceneNodeToParent(Node *rNode, Node *rNodeParent);
    void eraseSceneNode(Node *rNode);
    void addResourceNodeToSelectedItem(Node *rNode);
	void addSceneNodeToRootItem(Node *rNode);
	void addResourceNodeToRootItem(Node *rNode);
	void setSceneNodeAsSelected(Node *rNode);
	void clearScene();
	void clearResource();
	void clearSceneOpm();
	void clearResourceOpm();
	void updateResourceDropdownWithCurrent();
	void updateSceneDropdownWithCurrent();
	void setSceneEditable(bool isEditable);
	void setResourceEditable(bool isEditable);
	void setProjectAvailable(bool isActive);
	int getActiveTreeTabIndex();

private slots:
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

	void on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column);

	void on_tabWidget_currentChanged(int index);

	void on_sceneNameOpm_currentIndexChanged(const QString &arg1);

	void on_resourceNameOpm_currentIndexChanged(const QString &arg1);

	void on_sceneNameOpm_currentIndexChanged(int index);

	void on_resourceNameOpm_currentIndexChanged(int index);

	void on_newTexture_clicked();

	void on_newAnimation_clicked();

	void on_sceneNameOpm_activated(const QString &arg1);

	void on_resourceNameOpm_activated(const QString &arg1);

	void on_addLayer_clicked();

	void on_layerUp_clicked();

	void on_layerDown_clicked();

	void on_deleteLayer_clicked();

private:
	void addLayerNodeData(QTreeWidgetItem* r, NodeLayer* rNodeLayer);
	void moveLayerUpOrDown(bool moveUp);



	TabInfo		mTabInfoScene;
	TabInfo		mTabInfoResource;

	Ui::SceneTreeDock *ui;
	QTreeWidgetItem* addNodeX(QTreeWidgetItem *parent, Node* rNode, QTreeWidget* rQTreeWidget, bool expandTree);

	QTreeWidgetItem* getRootSceneItem();
	QTreeWidgetItem* getRootResourceItem();
	QTreeWidgetItem* getSelectedSceneItem();
	QTreeWidgetItem* getSelectedResourceItem();

};

#endif // SCENETREEDOCK_H
