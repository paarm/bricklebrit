#include "spriteeditor.h"
#include "ui_spriteeditor.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include "../treeutil.h"
#include "../previewimageutil.h"


SpriteEditor::SpriteEditor(Node *rNode, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SpriteEditor)
{
	ui->setupUi(this);
	mNode=static_cast<NodeSprite*>(rNode);
	ui->treeWidget->setColumnWidth(0, 350);

	mTexturePicker=new TexturePicker(ui->treeWidget);

	string currentResourceName="";
	if (mNode) {
		currentResourceName=mNode->getFrameRef().resourcefile;
		Node *rNodeReferenceNode=ProjectContext::getInstance().getFrameReferenceNodeForSprite(mNode);
		if (rNodeReferenceNode) {
			mSelectedItem=mTexturePicker->initialize(currentResourceName, rNodeReferenceNode);
			drawPreviewImage(false);
		}
	} else if (GuiContext::getInstance().getCurrentResource()) {
		currentResourceName=GuiContext::getInstance().getCurrentResource()->getName();
		mTexturePicker->switchToResource(currentResourceName);
	}

	vector<string> v=ProjectContext::getInstance().getResourceNames();
	int i=0;
	for (auto& s : v) {
		ui->resourceNameOpm->addItem(QString::fromStdString(s));
		if (s==currentResourceName) {
			ui->resourceNameOpm->setCurrentIndex(i);
		}
		i++;
	}
	if (mNode) {
		ui->spriteName->setText(QString::fromStdString(mNode->getName()));
		ui->spriteWidth->setValue(mNode->getSize().x);
		ui->spriteHeight->setValue(mNode->getSize().y);
	} else {
		ui->spriteName->setText("New Sprite");
	}
}


SpriteEditor::~SpriteEditor()
{
	delete ui;
	delete mTexturePicker;
}

void SpriteEditor::on_okButton_clicked()
{
	bool isNewNode=true;
	if (mNode) {
		isNewNode=false;
	} else {
		mNode=static_cast<NodeSprite*>(getInstanceFromNodeType(NodeType::Sprite, true));
	}
	if (mNode) {
		mNode->setName(ui->spriteName->text().toStdString());
		mNode->setSize(PointInt(ui->spriteWidth->value(), ui->spriteHeight->value()));
		mNode->getFrameRef().resourcefile=mSelectedItem.resourceName;
		mNode->getFrameRef().frame="";
		if (mSelectedItem.rNodeAnimation) {
			mNode->getFrameRef().textureid=mSelectedItem.rNodeAnimation->getId();
			if (mSelectedItem.rNodeAnimationFrame) {
				mNode->getFrameRef().frame=mSelectedItem.rNodeAnimationFrame->getFrameRef().frame;
			}
		} else if (mSelectedItem.rNodeTexture) {
			mNode->getFrameRef().textureid=mSelectedItem.rNodeTexture->getId();
			if (mSelectedItem.rNodeTextureFrame) {
				mNode->getFrameRef().frame=mSelectedItem.rNodeTextureFrame->getName();
			}
		}
		if (isNewNode) {
			GuiContext::getInstance().insertNewSceneNode(mNode);
		} else {
			GuiContext::getInstance().updateNodeName(mNode);
		}
	}

	this->deleteLater();
}


void SpriteEditor::on_cancelButton_clicked()
{
	this->deleteLater();
}

void SpriteEditor::on_SpriteEditor_rejected()
{
	this->deleteLater();
}

void SpriteEditor::on_resourceNameOpm_currentIndexChanged(const QString &arg1)
{
	ignoreparam(arg1);
}

void SpriteEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	ignoreparam(column);
	Node *rNode=TreeUtil::getNodeFromTreeItem(item);
	mSelectedItem=mTexturePicker->setSelectedNode(rNode);
	drawPreviewImage(true);
}

void SpriteEditor::drawPreviewImage(bool setWidthAndHeight) {
	if (mSelectedItem.rNodeTexture) {
		QPixmap pixmap=PreviewImageUtil::getPreviewImage(mSelectedItem, 200, 200);
		ui->previewImage->setPixmap(pixmap);

		if (mSelectedItem.rNodeTextureFrame) {
			if (setWidthAndHeight) {
				ui->spriteWidth->setValue(mSelectedItem.rNodeTextureFrame->getFrame().width);
				ui->spriteHeight->setValue(mSelectedItem.rNodeTextureFrame->getFrame().height);
			}
		} else {
			if (setWidthAndHeight) {
				BTexturePng *bTexture=ProjectContext::getInstance().getTexture(mSelectedItem.rNodeTexture->getPath());
				if (bTexture) {
					ui->spriteWidth->setValue(bTexture->width);
					ui->spriteHeight->setValue(bTexture->height);
				}
			}
		}
	}
}

void SpriteEditor::on_resourceNameOpm_activated(const QString &arg1)
{
	ignoreparam(arg1);
	QString rCurrentResource=ui->resourceNameOpm->currentText();
	mTexturePicker->switchToResource(rCurrentResource.toStdString());
}
