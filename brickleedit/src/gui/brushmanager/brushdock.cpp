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
}

void BrushDock::on_selectBrush_clicked()
{
	BrushSelection *rBrushSelection=new BrushSelection(mSelectedItem.resourceName, getSelectedBrushNode(), this);
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
	mSelectedItem.clear();
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

void BrushDock::setAsBrush(SelectedItem rSelectedItem, SelectedItemPref *rSelectedItemPref) {
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
    ui->scaleX->setValue(1.0);
    ui->scaleY->setValue(1.0);
    ui->flipX->setChecked(false);
    ui->flipY->setChecked(false);
    ui->rotate->setValue(0.0);

	if (rSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(rSelectedItem.rNodeTexture, rSelectedItem.rNodeTextureFrame, 100, 100);
		ui->selectBrush->setIcon(pixmap);
		//ui->previewImage->setPixmap(pixmap);
		mSelectedItem=rSelectedItem;

        if (rSelectedItemPref) {
            ui->sizeX->setValue(rSelectedItemPref->sizeWH.x);
            ui->sizeY->setValue(rSelectedItemPref->sizeWH.y);
            ui->scaleX->setValue(rSelectedItemPref->scale.x);
            ui->scaleY->setValue(rSelectedItemPref->scale.y);
            ui->rotate->setValue(rSelectedItemPref->rotation);
            ui->flipX->setChecked(rSelectedItemPref->flipX);
            ui->flipY->setChecked(rSelectedItemPref->flipY);
        } else {
            //ui->currentBrush->setText(rSelected);
            BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rSelectedItem.rNodeTexture->getPath());
            if (bTexture) {
                if (rSelectedItem.rNodeTextureFrame) {
                    ui->sizeX->setValue(rSelectedItem.rNodeTextureFrame->getFrame().width);
                    ui->sizeY->setValue(rSelectedItem.rNodeTextureFrame->getFrame().height);
                } else {
                    ui->sizeX->setValue(bTexture->width);
                    ui->sizeY->setValue(bTexture->height);
                }
            }
        }
	}
}

SelectedItem& BrushDock::getSelectedBrush() {
	return mSelectedItem;
}

Node* BrushDock::getSelectedBrushNode() {
	if (mSelectedItem.rNodeAnimationFrame) {
		return mSelectedItem.rNodeAnimationFrame;
	} else if (mSelectedItem.rNodeTextureFrame) {
		return mSelectedItem.rNodeTextureFrame;
	} else {
		return mSelectedItem.rNodeTexture;
	}
}

void BrushDock::setCurrentPaintCanvas(Node2d *rNode2d) {
	if (rNode2d) {
		ui->canvasNodeName->setText(QString::fromStdString(rNode2d->getName()));
	} else {
		ui->canvasNodeName->setText("");
	}
	mCurrentPaintCanvas=rNode2d;
}

Node2d* BrushDock::getCurrentPaintCanvas() {
    return mCurrentPaintCanvas;
}

int BrushDock::getBrushWidth() {
	return ui->sizeX->value();
}

int BrushDock::getBrushHeight() {
	return ui->sizeY->value();
}

bool BrushDock::getBrushFlipX() {
    return ui->flipX->isChecked();
}

bool BrushDock::getBrushFlipY() {
    return ui->flipY->isChecked();
}

PointInt BrushDock::getBrushSize() {
	return PointInt(getBrushWidth(), getBrushHeight());
}

PointFloat BrushDock::getBrushScale() {
    return PointFloat(ui->scaleX->value(), ui->scaleY->value());
}

float BrushDock::getRotation() {
	return static_cast<float>(ui->rotate->value());
}
//GLMVector3 v=GuiContext::getInstance().unprojectedScreenCoord(mSceneMouseInfo.x, mSceneMouseInfo.y);

NodeSprite* BrushDock::getNewNodeFromBrush(float worldX, float worldY) {
	NodeSprite *rv=nullptr;
	NodeSprite* rvv=getNodeFromBrush(worldX, worldY);
	if (rvv) {
		rv=new NodeSprite();

		rv->setPosition(rvv->getPosition());
		rv->setSize(rvv->getSize());
		rv->setScale(rvv->getScale());
		rv->setRotation(rvv->getRotation());
		rv->setFrameRef(rvv->getFrameRef());
        rv->setFlipX(rvv->getFlipX());
        rv->setFlipY(rvv->getFlipY());
        rv->setName("Sprite");
	}
	return rv;
}


NodeSprite* BrushDock::getNodeFromBrush(float worldX, float worldY) {
	NodeSprite *rv=nullptr;
	if (GuiContext::getInstance().getCurrentPaintCanvas() &&
		GuiContext::getInstance().getCurrentTool()==Tool::Brush &&
		getSelectedBrushNode()) {

		glm::mat4 parentMatrix=glm::make_mat4(GuiContext::getInstance().getCurrentPaintCanvas()->getCurrentModelMatrix().getPointer());
		glm::mat4 reverseMatrix=glm::inverse(parentMatrix);
		glm::vec4 v4(worldX, worldY, 0.0, 1.0);
		v4=reverseMatrix*v4;
		PointInt pp(static_cast<int>(v4.x), static_cast<int>(v4.y));

		if (GuiContext::getInstance().isGridActive()) {
			PointInt gridSize;
			PointInt gridOffset;
			if (ProjectContext::getInstance().getNodeProject()) {
				gridSize=ProjectContext::getInstance().getNodeProject()->getGridSize();
				gridOffset=ProjectContext::getInstance().getNodeProject()->getGridOffset();
			}
			pp.x=WorldCalculator::calcGridPos(pp.x, gridSize.x, gridOffset.x);
			pp.y=WorldCalculator::calcGridPos(pp.y, gridSize.y, gridOffset.y);
        }
		mNodeFromBrush.setPosition(pp);
		mNodeFromBrush.setSize(GuiContext::getInstance().getMainWindow().getBrushDock().getBrushSize());
		mNodeFromBrush.setScale(GuiContext::getInstance().getMainWindow().getBrushDock().getBrushScale());
		mNodeFromBrush.setRotation(GuiContext::getInstance().getMainWindow().getBrushDock().getRotation());
        mNodeFromBrush.setFlipX(getBrushFlipX());
        mNodeFromBrush.setFlipY(getBrushFlipY());
        SelectedItem rSelectedItem = GuiContext::getInstance().getMainWindow().getBrushDock().getSelectedBrush();
		if (rSelectedItem.rNodeAnimationFrame) {
			mNodeFromBrush.getFrameRef().frame=rSelectedItem.rNodeAnimationFrame->getFrameRef().frame;
			mNodeFromBrush.getFrameRef().resourcefile=rSelectedItem.rNodeAnimationFrame->getFrameRef().resourcefile;
			mNodeFromBrush.getFrameRef().textureid=rSelectedItem.rNodeAnimationFrame->getFrameRef().textureid;
		} else if(rSelectedItem.rNodeTextureFrame) {
			mNodeFromBrush.getFrameRef().frame=rSelectedItem.rNodeTextureFrame->getName();
			mNodeFromBrush.getFrameRef().resourcefile=rSelectedItem.resourceName;
			mNodeFromBrush.getFrameRef().textureid=rSelectedItem.rNodeTexture->getId();
		} else if (rSelectedItem.rNodeTexture) {
			mNodeFromBrush.getFrameRef().resourcefile=rSelectedItem.resourceName;
			mNodeFromBrush.getFrameRef().textureid=rSelectedItem.rNodeTexture->getId();
			mNodeFromBrush.getFrameRef().frame="";
		}
		rv=&mNodeFromBrush;
	}
	return rv;
}
