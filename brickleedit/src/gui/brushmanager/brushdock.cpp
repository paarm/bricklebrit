#include "brushdock.h"
#include "ui_brushdock.h"
#include "brushselection.h"
#include "../previewimageutil.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../scene/worldcalculator.h"

BrushDock::BrushDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::BrushDock)
{
	ui->setupUi(this);
	setBrushEnabled(false);
}

BrushDock::~BrushDock()
{
	delete ui;
	for (NodeSprite* rNodeSprite : mNodeFromBrushListCache) {
		delete rNodeSprite;
	}
	mNodeFromBrushListCache.clear();
}

void BrushDock::on_selectBrush_clicked()
{
	string resourceName;
	if (!mBrushInfoItemList.empty()) {
		resourceName=mBrushInfoItemList.at(mBrushInfoCenterIndex).rSelectedItem.resourceName;
	}
	BrushSelection *rBrushSelection=new BrushSelection(resourceName, getSelectedBrushNode(), this);
	rBrushSelection->show();
}

void BrushDock::clearBrush() {
	//ui->previewImage->setPixmap(QPixmap());
	ui->selectBrush->setIcon(QIcon(":icons/brush.png"));
	ui->scaleX->setValue(1.0);
    ui->scaleY->setValue(1.0);
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
	ui->rotate->setValue(0);
    ui->flipX->setChecked(false);
    ui->flipY->setChecked(false);
	mBrushInfoItemList.clear();
	mBrushInfoCenterIndex=0;
}

void BrushDock::setBrushEnabled(bool enabled) {
	if (!enabled) {
		clearBrush();
	}
	ui->selectBrush->setEnabled(enabled);
    ui->scaleX->setEnabled(enabled);
    ui->scaleY->setEnabled(enabled);
    ui->sizeX->setEnabled(enabled);
	ui->sizeY->setEnabled(enabled);
    ui->flipX->setEnabled(enabled);
    ui->flipY->setEnabled(enabled);
    ui->rotate->setEnabled(enabled);
}

void BrushDock::setCurrentSelectionAsBrush() {
	vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
	setNodesAsBrush(v);
}

void BrushDock::setNodesAsBrush(vector<Node*> v) {
	mBrushInfoItemList.clear();
	mBrushInfoCenterIndex=0;
	mBrushIsInvalid=true;
	Node *rNode=GuiContext::getInstance().getSelectionManager().getLatestSelected();

	if (rNode && rNode->getNodeType()==NodeType::Sprite) {
		NodeSprite * rNodeSpriteCenter=static_cast<NodeSprite*>(rNode);
		for (Node*n : v) {
			if (n->getNodeType()==NodeType::Sprite) {
				NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);

				Node * rNodeTextureOrAnimation=GuiContext::getInstance().getFrameReferenceNodeForSprite(rNodeSprite);
				if (rNodeTextureOrAnimation) {
					BrushInfoItem rBrushInfoItem;
					rBrushInfoItem.rSelectedItem=GuiContext::getInstance().prepareSelectedNodeFromTextureOrAnimationNode(rNodeTextureOrAnimation, rNodeSprite->getFrameRef().resourcefile);
					rBrushInfoItem.rSelectedItemPref.offsetX=0;
					rBrushInfoItem.rSelectedItemPref.offsetY=0;
					rBrushInfoItem.rSelectedItemPref.sizeWH=rNodeSprite->getSize();
					rBrushInfoItem.rSelectedItemPref.scale=rNodeSprite->getScale();
					rBrushInfoItem.rSelectedItemPref.rotation=rNodeSprite->getRotation();
					rBrushInfoItem.rSelectedItemPref.flipX=rNodeSprite->getFlipX();
					rBrushInfoItem.rSelectedItemPref.flipY=rNodeSprite->getFlipY();
					if (rNodeSprite!=rNodeSpriteCenter) {
						rBrushInfoItem.rSelectedItemPref.offsetX=rNodeSprite->getCurrentWorldLocationCenter().x-rNodeSpriteCenter->getCurrentWorldLocationCenter().x;
						rBrushInfoItem.rSelectedItemPref.offsetY=rNodeSprite->getCurrentWorldLocationCenter().y-rNodeSpriteCenter->getCurrentWorldLocationCenter().y;
					} else {
						mBrushInfoCenterIndex=mBrushInfoItemList.size();
						setCenterBrushUIInfo(rBrushInfoItem);
					}
					mBrushInfoItemList.push_back(rBrushInfoItem);
				}
			}
		}
	}
}

void BrushDock::setSelectedItemAsBrush(SelectedItem rSelectedItem) {
	mBrushInfoItemList.clear();
	mBrushInfoCenterIndex=0;
	mBrushIsInvalid=true;
	BrushInfoItem rBrushInfoItem;
	rBrushInfoItem.rSelectedItem=rSelectedItem;
	rBrushInfoItem.rSelectedItemPref.scale=PointFloat(1.0,1.0);
	rBrushInfoItem.rSelectedItemPref.flipX=false;
	rBrushInfoItem.rSelectedItemPref.flipY=false;
	rBrushInfoItem.rSelectedItemPref.offsetX=0;
	rBrushInfoItem.rSelectedItemPref.offsetY=0;
	rBrushInfoItem.rSelectedItemPref.sizeWH=PointInt(0,0);
	rBrushInfoItem.rSelectedItemPref.rotation=0;
	BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rBrushInfoItem.rSelectedItem.rNodeTexture->getPath());
	if (bTexture) {
		if (rBrushInfoItem.rSelectedItem.rNodeTextureFrame) {
			rBrushInfoItem.rSelectedItemPref.sizeWH=PointInt(rBrushInfoItem.rSelectedItem.rNodeTextureFrame->getFrame().width, rBrushInfoItem.rSelectedItem.rNodeTextureFrame->getFrame().height);
		} else {
			rBrushInfoItem.rSelectedItemPref.sizeWH=PointInt(bTexture->width, bTexture->height);
		}
	}
	setCenterBrushUIInfo(rBrushInfoItem);
	mBrushInfoCenterIndex=mBrushInfoItemList.size();
	mBrushInfoItemList.push_back(rBrushInfoItem);
}

void BrushDock::setCenterBrushUIInfo(BrushInfoItem& rBrushInfoItem) {
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
    ui->scaleX->setValue(1.0);
    ui->scaleY->setValue(1.0);
    ui->flipX->setChecked(false);
    ui->flipY->setChecked(false);
    ui->rotate->setValue(0.0);

	if (rBrushInfoItem.rSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(rBrushInfoItem.rSelectedItem.rNodeTexture, rBrushInfoItem.rSelectedItem.rNodeTextureFrame, 50, 50);
		ui->selectBrush->setIcon(pixmap);

		ui->sizeX->setValue(rBrushInfoItem.rSelectedItemPref.sizeWH.x);
		ui->sizeY->setValue(rBrushInfoItem.rSelectedItemPref.sizeWH.y);
		ui->scaleX->setValue(rBrushInfoItem.rSelectedItemPref.scale.x);
		ui->scaleY->setValue(rBrushInfoItem.rSelectedItemPref.scale.y);
		ui->rotate->setValue(rBrushInfoItem.rSelectedItemPref.rotation);
		ui->flipX->setChecked(rBrushInfoItem.rSelectedItemPref.flipX);
		ui->flipY->setChecked(rBrushInfoItem.rSelectedItemPref.flipY);
	}
}


Node* BrushDock::getSelectedBrushNode() {
	if (!mBrushInfoItemList.empty()) {
		BrushInfoItem &rBrushInfoItemCenter=mBrushInfoItemList.at(mBrushInfoCenterIndex);
		if (rBrushInfoItemCenter.rSelectedItem.rNodeAnimationFrame) {
			return rBrushInfoItemCenter.rSelectedItem.rNodeAnimationFrame;
		} else if (rBrushInfoItemCenter.rSelectedItem.rNodeTextureFrame) {
			return rBrushInfoItemCenter.rSelectedItem.rNodeTextureFrame;
		} else {
			return rBrushInfoItemCenter.rSelectedItem.rNodeTexture;
		}
	}
	return nullptr;
}


vector<NodeSprite *> BrushDock::getNewNodesFromBrush(float worldX, float worldY) {
	vector<NodeSprite *>rv;
	vector<NodeSprite*>& v=getNodesFromBrush(worldX, worldY);
	for (NodeSprite* rNodeSprite : v) {
		NodeSprite *rNodeSprite_new=new NodeSprite();

		rNodeSprite_new->setPosition(rNodeSprite->getPosition());
		rNodeSprite_new->setSize(rNodeSprite->getSize());
		rNodeSprite_new->setScale(rNodeSprite->getScale());
		rNodeSprite_new->setRotation(rNodeSprite->getRotation());
		rNodeSprite_new->setFrameRef(rNodeSprite->getFrameRef());
		rNodeSprite_new->setFlipX(rNodeSprite->getFlipX());
		rNodeSprite_new->setFlipY(rNodeSprite->getFlipY());
		rNodeSprite_new->setName("Sprite");
		rv.push_back(rNodeSprite_new);
	}
	return rv;
}

vector<NodeSprite*>& BrushDock::getNodesFromBrush(float worldX, float worldY) {
	if (GuiContext::getInstance().getCurrentPaintCanvas() &&
		GuiContext::getInstance().getCurrentTool()==Tool::Brush &&
		getSelectedBrushNode()) {

		size_t cntBrushes=mBrushInfoItemList.size();
		size_t cntSpriteCache=mNodeFromBrushListCache.size();
		int missingSpriteCacheEntries=cntBrushes-cntSpriteCache;
		if (missingSpriteCacheEntries>0) {
			for (int i=0;i<missingSpriteCacheEntries;i++) {
				mNodeFromBrushListCache.push_back(new NodeSprite());
			}
		}
		if (mBrushIsInvalid) {
			mNodeFromBrushList.clear();
			int i=0;
			for (BrushInfoItem& rBrushInfoItem : mBrushInfoItemList) {
				NodeSprite* rNodeSprite=mNodeFromBrushListCache.at(i);
				PointInt pp=WorldCalculator::getLocalPosFromWorldPos(GuiContext::getInstance().getCurrentPaintCanvas(), PointFloat(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY), true);

				rNodeSprite->setPosition(pp);
				if (i==int(mBrushInfoCenterIndex)) {
					rNodeSprite->setSize(PointInt(ui->sizeX->value(), ui->sizeY->value()));
					rNodeSprite->setScale(PointFloat(ui->scaleX->value(), ui->scaleY->value()));
					rNodeSprite->setRotation(ui->rotate->value());
					rNodeSprite->setFlipX(ui->flipX->isChecked());
					rNodeSprite->setFlipY(ui->flipY->isChecked());
				} else {
					rNodeSprite->setSize(rBrushInfoItem.rSelectedItemPref.sizeWH);
					rNodeSprite->setScale(rBrushInfoItem.rSelectedItemPref.scale);
					rNodeSprite->setRotation(rBrushInfoItem.rSelectedItemPref.rotation);
					rNodeSprite->setFlipX(rBrushInfoItem.rSelectedItemPref.flipX);
					rNodeSprite->setFlipY(rBrushInfoItem.rSelectedItemPref.flipY);
				}
				if (rBrushInfoItem.rSelectedItem.rNodeAnimationFrame) {
					rNodeSprite->getFrameRef().frame=rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().frame;
					rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile;
					rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid;
				} else if(rBrushInfoItem.rSelectedItem.rNodeTextureFrame) {
					rNodeSprite->getFrameRef().frame=rBrushInfoItem.rSelectedItem.rNodeTextureFrame->getName();
					rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.resourceName;
					rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeTexture->getId();
				} else if (rBrushInfoItem.rSelectedItem.rNodeTexture) {
					rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.resourceName;
					rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeTexture->getId();
					rNodeSprite->getFrameRef().frame="";
				}
				mNodeFromBrushList.push_back(rNodeSprite);
				i++;
			}
		} else {
			int i=0;
			for(NodeSprite* rNodeSprite: mNodeFromBrushList) {
				BrushInfoItem& rBrushInfoItem=mBrushInfoItemList.at(i);
				PointInt pp=WorldCalculator::getLocalPosFromWorldPos(GuiContext::getInstance().getCurrentPaintCanvas(), PointFloat(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY), true);
				rNodeSprite->setPosition(pp);
				if (i==int(mBrushInfoCenterIndex)) {
					rNodeSprite->setSize(PointInt(ui->sizeX->value(), ui->sizeY->value()));
					rNodeSprite->setScale(PointFloat(ui->scaleX->value(), ui->scaleY->value()));
					rNodeSprite->setRotation(ui->rotate->value());
					rNodeSprite->setFlipX(ui->flipX->isChecked());
					rNodeSprite->setFlipY(ui->flipY->isChecked());
				}
				i++;
			}
		}
		mBrushIsInvalid=false;
	}
	return mNodeFromBrushList;
}
