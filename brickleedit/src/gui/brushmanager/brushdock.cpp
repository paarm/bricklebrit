#include "brushdock.h"
#include "ui_brushdock.h"
#include "brushselection.h"
#include "../previewimageutil.h"
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
	setBrushPossible(false);
}

BrushDock::~BrushDock()
{
	delete ui;
	mCurrentBrushNodes.clear();
	mCurrentPaintCanvas=nullptr;
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
	removeBrushNodes();
}

void BrushDock::setBrushPossible(bool enabled) {
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

void BrushDock::BrushToolActivated(bool activated) {
	if (activated) {
		createNewBrushNodes();
	} else {
		removeBrushNodes();
	}
}

void BrushDock::setCurrentPaintCanvas(Node2d *rNode2d) {
	if (mCurrentPaintCanvas!=rNode2d || rNode2d==nullptr) {
		removeBrushNodes();
		mCurrentPaintCanvas=rNode2d;
		createNewBrushNodes();
	}
}

Node2d* BrushDock::getCurrentPaintCanvas() {
	return mCurrentPaintCanvas;
}

void BrushDock::setCurrentSelectionAsBrush() {
	removeBrushNodes();
	vector<Node*> &v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
	setNodesAsBrush(v);
	createNewBrushNodes();
}

void BrushDock::setNodesAsBrush(vector<Node*> &v) {
	mBrushInfoItemList.clear();
	mBrushInfoCenterIndex=0;
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
						rBrushInfoItem.rSelectedItemPref.offsetX=rNodeSprite->getLocationInfo().rWorldLocationCenter.x-rNodeSpriteCenter->getLocationInfo().rWorldLocationCenter.x;
						rBrushInfoItem.rSelectedItemPref.offsetY=rNodeSprite->getLocationInfo().rWorldLocationCenter.y-rNodeSpriteCenter->getLocationInfo().rWorldLocationCenter.y;
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
	removeBrushNodes();

	mBrushInfoItemList.clear();
	mBrushInfoCenterIndex=0;
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
	createNewBrushNodes();
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

void BrushDock::removeBrushNodes() {
	if (mCurrentPaintCanvas && mCurrentBrushNodes.size()>0) {
		for (NodeSprite* rNodeSprite: mCurrentBrushNodes) {
			mCurrentPaintCanvas->deleteChildNode(rNodeSprite);
		}
	}
	mCurrentBrushNodes.clear();
}

void BrushDock::updateBrushNodesPosition(float worldX, float worldY) {
	if (mCurrentPaintCanvas &&
		mCurrentBrushNodes.size()>0 &&
		GuiContext::getInstance().getCurrentTool()==Tool::Brush) {
		int i=0;
		for (NodeSprite* rNodeSprite : mCurrentBrushNodes) {
			BrushInfoItem& rBrushInfoItem=mBrushInfoItemList.at(i);
			glm::vec4 pv=mCurrentPaintCanvas->getLocalPosFromWorldPos(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY, GuiContext::getInstance().isGridActive());
			//PointInt pp=WorldCalculator::getLocalPosFromWorldPos(GuiContext::getInstance().getCurrentPaintCanvas(), PointFloat(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY), true);
			PointInt pp(pv.x, pv.y);
			rNodeSprite->setPosition(pp);
			i++;
		}
	}
}

void BrushDock::commitBrushNodes() {
	if (mCurrentPaintCanvas &&
		mCurrentBrushNodes.size()>0 &&
		GuiContext::getInstance().getCurrentTool()==Tool::Brush) {
		mCurrentBrushNodes.clear();
		createNewBrushNodes();
	}
}

void BrushDock::createNewBrushNodes() {
	//removeBrushNodes();
	if (GuiContext::getInstance().getCurrentTool()==Tool::Brush &&
		mCurrentPaintCanvas &&
		mBrushInfoItemList.size()>0) {
		float worldX=0.0;
		float worldY=0.0;

		int i=0;
		for (BrushInfoItem& rBrushInfoItem : mBrushInfoItemList) {
			NodeSprite* rNodeSprite=new NodeSprite();
			glm::vec4 pv=GuiContext::getInstance().getCurrentPaintCanvas()->getLocalPosFromWorldPos(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY, GuiContext::getInstance().isGridActive());
			//PointInt pp=WorldCalculator::getLocalPosFromWorldPos(GuiContext::getInstance().getCurrentPaintCanvas(), PointFloat(worldX+rBrushInfoItem.rSelectedItemPref.offsetX, worldY+rBrushInfoItem.rSelectedItemPref.offsetY), true);
			PointInt pp(pv.x, pv.y);
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
				rNodeSprite->setFrameRef(FrameRef(rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile, rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid, rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().frame));
				//rNodeSprite->getFrameRef().frame=;
				//rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile;
				//rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid;
			} else if(rBrushInfoItem.rSelectedItem.rNodeTextureFrame) {
				rNodeSprite->setFrameRef(FrameRef(rBrushInfoItem.rSelectedItem.resourceName, rBrushInfoItem.rSelectedItem.rNodeTexture->getId(), rBrushInfoItem.rSelectedItem.rNodeTextureFrame->getName()));
				//rNodeSprite->getFrameRef().frame=rBrushInfoItem.rSelectedItem.rNodeTextureFrame->getName();
				//rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.resourceName;
				//rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeTexture->getId();
			} else if (rBrushInfoItem.rSelectedItem.rNodeTexture) {
				rNodeSprite->setFrameRef(FrameRef(rBrushInfoItem.rSelectedItem.resourceName, rBrushInfoItem.rSelectedItem.rNodeTexture->getId(), ""));
				//rNodeSprite->getFrameRef().resourcefile=rBrushInfoItem.rSelectedItem.resourceName;
				//rNodeSprite->getFrameRef().textureid=rBrushInfoItem.rSelectedItem.rNodeTexture->getId();
				//rNodeSprite->getFrameRef().frame="";
			}
			mCurrentBrushNodes.push_back(rNodeSprite);
			mCurrentPaintCanvas->addChildNode(rNodeSprite);
			i++;
		}
	}
}


void BrushDock::on_flipX_toggled(bool checked)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setFlipX(checked);
		mBrushInfoItemList.at(i).rSelectedItemPref.flipX=checked;
		i++;
	}
}

void BrushDock::on_flipY_toggled(bool checked)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setFlipY(checked);
		mBrushInfoItemList.at(i).rSelectedItemPref.flipY=checked;
		i++;
	}
}

void BrushDock::on_rotate_valueChanged(int arg1)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setRotation(arg1);
		mBrushInfoItemList.at(i).rSelectedItemPref.rotation=arg1;
		i++;
	}
}

void BrushDock::on_scaleX_valueChanged(double arg1)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setScale(PointFloat(arg1, n->getScale().y));
		mBrushInfoItemList.at(i).rSelectedItemPref.scale=n->getScale();
		i++;
	}
}

void BrushDock::on_scaleY_valueChanged(double arg1)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setScale(PointFloat(n->getScale().x, arg1));
		mBrushInfoItemList.at(i).rSelectedItemPref.scale=n->getScale();
		i++;
	}
}

void BrushDock::on_sizeX_valueChanged(int arg1)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setSize(PointInt(arg1, n->getSize().y));
		mBrushInfoItemList.at(i).rSelectedItemPref.sizeWH=n->getSize();
		i++;
	}
}

void BrushDock::on_sizeY_valueChanged(int arg1)
{
	int i=0;
	for(Node2d* n : mCurrentBrushNodes) {
		n->setSize(PointInt(n->getSize().x, arg1));
		mBrushInfoItemList.at(i).rSelectedItemPref.sizeWH=n->getSize();
		i++;
	}
}
