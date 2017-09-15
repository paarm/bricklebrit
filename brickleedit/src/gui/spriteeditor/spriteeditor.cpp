#include "spriteeditor.h"
#include "ui_spriteeditor.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include "../treeutil.h"

SpriteEditor::SpriteEditor(Node *rNode, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SpriteEditor)
{
	ui->setupUi(this);
	mNode=static_cast<NodeSprite*>(rNode);
	setupNode();
}


SpriteEditor::~SpriteEditor()
{
	delete ui;
}

void SpriteEditor::setupNode() {
	string currentResourceName="";
	if (mNode) {
		currentResourceName=mNode->getFrameRef().resourcefile;
		ui->spriteName->setText(QString::fromStdString(mNode->getName()));
	} else {
		ui->spriteName->setText("New Sprite");
	}
	if (currentResourceName.size()==0) {
		if (GuiContext::getInstance().getCurrentResource()) {
			currentResourceName=GuiContext::getInstance().getCurrentResource()->getName();
		}
	}
	vector<string> v=ProjectContext::getInstance().getFileNamesForType(NodeInfoType::Resource);
	int i=0;
	for (auto& s : v) {
		ui->resourceNameOpm->addItem(QString::fromStdString(s));
		if (s==currentResourceName) {
			ui->resourceNameOpm->setCurrentIndex(i);
		}
		i++;
	}
	if (mNode) {
		ui->spriteWidth->setValue(mNode->getSize().x);
		ui->spriteHeight->setValue(mNode->getSize().y);
		int cnt=ui->treeWidget->topLevelItemCount();
		for (int i=0;i<cnt;i++) {
			QTreeWidgetItem *r=ui->treeWidget->topLevelItem(i);
			Node *rNode=TreeUtil::getNodeFromTreeItem(r);
			if (rNode && rNode->getId()==mNode->getFrameRef().textureid) {
				if (!mNode->getFrameRef().frame.empty()) {
					if (rNode->getNodeType()==NodeType::Texture || rNode->getNodeType()==NodeType::Animation) {
						for (int x=0;x<cnt;x++) {
							QTreeWidgetItem *c=r->child(x);
							if (c) {
								Node *rNodeC=TreeUtil::getNodeFromTreeItem(c);
								if (rNodeC && rNodeC->getName()==mNode->getFrameRef().frame) {
									updateSelectedFrame(rNodeC, false);
									break;
								}
							}
						}
					}
				} else {
					updateSelectedFrame(rNode, false);
				}
				break;
			}
		}
	}
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
		mNode->getFrameRef().resourcefile=mCurrentRef.resourceName;
		mNode->getFrameRef().textureid=mCurrentRef.id;
		mNode->getFrameRef().frame=mCurrentRef.frameName;
		if (isNewNode) {
			GuiContext::getInstance().insertNewNode(mNode, NodeInfoType::Scene);
		} else {
			GuiContext::getInstance().updateNodeName(mNode, NodeInfoType::Scene);
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
	QString rCurrentResource=ui->resourceNameOpm->currentText();
	TreeUtil::fillTreeWidgetWithTexturesFromResource(ui->treeWidget, rCurrentResource.toStdString(), true, true);
}

void SpriteEditor::updateSelectedFrame(Node *rNode, bool setWidthAndHeight) {
	mCurrentRef.resourceName=ui->resourceNameOpm->currentText().toStdString();
	mCurrentRef.frameName="";
	if (rNode) {
		NodeTexture *rNodeTexture=nullptr;
		NodeTextureFrame *rNodeTextureFrame=nullptr;
		if (rNode->getNodeType()==NodeType::Texture) {
			mCurrentRef.id=rNode->getId();
			rNodeTexture=static_cast<NodeTexture*>(rNode);
		} else if (rNode->getNodeType()==NodeType::TextureFrame) {
			rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
			mCurrentRef.frameName=rNodeTextureFrame->getName();
			rNodeTexture=static_cast<NodeTexture*>(rNode->getParent());
			mCurrentRef.id=rNodeTexture->getId();
		}
		if (rNodeTexture) {
			BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
			if (bTexture) {
				QImage rImageSrc(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
				if (rNodeTextureFrame) {
					QImage rImageFrame=rImageSrc.copy(rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y, rNodeTextureFrame->getFrame().width, rNodeTextureFrame->getFrame().height);
					rImageFrame=rImageFrame.scaled(QSize(100,100), Qt::KeepAspectRatio);
					ui->previewImage->setPixmap(QPixmap::fromImage(rImageFrame));
					if (setWidthAndHeight) {
						ui->spriteWidth->setValue(rNodeTextureFrame->getFrame().width);
						ui->spriteHeight->setValue(rNodeTextureFrame->getFrame().height);
					}
				} else {
					QImage rImageFrame=rImageSrc.scaled(QSize(100,100), Qt::KeepAspectRatio);
					ui->previewImage->setPixmap(QPixmap::fromImage(rImageFrame));
					if (setWidthAndHeight) {
						ui->spriteWidth->setValue(bTexture->width);
						ui->spriteHeight->setValue(bTexture->height);
					}
				}
			}
		}
	}
}

void SpriteEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	Node *rNode=TreeUtil::getNodeFromTreeItem(item);
	updateSelectedFrame(rNode, true);
}
