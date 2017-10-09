#include "brushdock.h"
#include "ui_brushdock.h"
#include "brushselection.h"
#include "../previewimageutil.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	ui->previewImage->setPixmap(QPixmap());
	ui->scale->setValue(1.0);
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
	ui->rotate->setValue(0);
	ui->currentBrush->setText("");
}

void BrushDock::setBrushEnabled(bool enabled) {
	if (!enabled) {
		clearBrush();
	}
	ui->selectBrush->setEnabled(enabled);
	ui->scale->setEnabled(enabled);
	ui->sizeX->setEnabled(enabled);
	ui->sizeY->setEnabled(enabled);
	ui->rotate->setEnabled(enabled);
    ui->gridActive->setEnabled(enabled);
    setGridFieldsState();
}

void BrushDock::setAsBrush(SelectedItem rSelectedItem, SelectedItemPref *rSelectedItemPref) {
	ui->sizeX->setValue(0);
	ui->sizeY->setValue(0);
    ui->scale->setValue(1.0);
    ui->rotate->setValue(0.0);

	if (rSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(rSelectedItem, 100, 100);
		ui->previewImage->setPixmap(pixmap);
		mSelectedItem=rSelectedItem;

        if (rSelectedItemPref) {
            ui->sizeX->setValue(rSelectedItemPref->sizeWH.x);
            ui->sizeY->setValue(rSelectedItemPref->sizeWH.y);
            ui->scale->setValue(rSelectedItemPref->scale.x);
            ui->rotate->setValue(rSelectedItemPref->rotation);
        } else {
            //ui->currentBrush->setText(rSelected);
            BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rSelectedItem.rNodeTexture->getPath());
            if (bTexture) {
                if (rSelectedItem.rNodeTextureFrame) {
                    ui->sizeX->setValue(rSelectedItem.rNodeTextureFrame->getFrame().width);
                    ui->sizeY->setValue(rSelectedItem.rNodeTextureFrame->getFrame().height);
                    ui->scale->setValue(1.0);
                } else {
                    ui->sizeX->setValue(bTexture->width);
                    ui->sizeY->setValue(bTexture->height);
                    ui->scale->setValue(1.0);
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
	return nullptr;
}

void BrushDock::setCurrentPaintCanvas(Node2d *rNode2d) {
	if (rNode2d) {
		ui->canvasNodeName->setText(QString::fromStdString(rNode2d->getName()));
	} else {
		ui->canvasNodeName->setText("");
	}
	mCurrentPaintCanvas=rNode2d;
}

int BrushDock::getBrushWidth() {
	return ui->sizeX->value();
}

int BrushDock::getBrushHeight() {
	return ui->sizeY->value();
}

PointInt BrushDock::getBrushSize() {
	return PointInt(getBrushWidth(), getBrushHeight());
}

PointFloat BrushDock::getBrushScale() {
	return PointFloat(ui->scale->value(), ui->scale->value());
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
		rv->setName("Sprite");
	}
	return rv;
}

int BrushDock::calcGridPos(int worldPos, int gridSize, int gridOffset) {
    int rv=worldPos;
    if (gridSize>0) {
        int gridField=0;
        if (worldPos>0) {
            gridField=(worldPos+gridSize/2)/gridSize;
        } else if (worldPos<0) {
            gridField=(worldPos-gridSize/2)/gridSize;
        }
        rv=gridField*gridSize;//+rGridState.gridX/2;
        if (gridOffset>0) {
            if (worldPos>=0) {
                rv+=gridOffset;
            } else {
                rv-=gridOffset;
            }
        }
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

        GridState rGridState=getGridState();
        if (rGridState.isActive) {
            pp.x=calcGridPos(pp.x, rGridState.gridX, rGridState.offsetX);
            pp.y=calcGridPos(pp.y, rGridState.gridY, rGridState.offsetY);
        }
		mNodeFromBrush.setPosition(pp);
		mNodeFromBrush.setSize(GuiContext::getInstance().getMainWindow().getBrushDock().getBrushSize());
		mNodeFromBrush.setScale(GuiContext::getInstance().getMainWindow().getBrushDock().getBrushScale());
		mNodeFromBrush.setRotation(GuiContext::getInstance().getMainWindow().getBrushDock().getRotation());
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

void BrushDock::setGridFieldsState() {
    bool enable=false;
    if (ui->gridActive->isEnabled() && ui->gridActive->isChecked()) {
        enable=true;
    }
    ui->gridX->setEnabled(enable);
    ui->gridY->setEnabled(enable);
    ui->offsetX->setEnabled(enable);
    ui->offsetY->setEnabled(enable);
}

void BrushDock::on_gridActive_clicked(bool checked)
{
    ignoreparam(checked);
    setGridFieldsState();
}

GridState BrushDock::getGridState() {
    GridState rGridState;
    rGridState.isActive=ui->gridActive->isChecked();
    rGridState.gridX=ui->gridX->value();
    rGridState.gridY=ui->gridY->value();
    rGridState.offsetX=ui->offsetX->value();
    rGridState.offsetY=ui->offsetY->value();
    return rGridState;
}

