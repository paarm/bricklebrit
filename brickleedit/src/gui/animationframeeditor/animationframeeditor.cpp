#include "animationframeeditor.h"
#include "ui_animationframeeditor.h"
#include "../../project/projectcontext.h"
#include "../guicontext.h"
#include "../treeutil.h"

AnimationFrameEditor::AnimationFrameEditor(Node *rNode, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AnimationFrameEditor)
{
	mNode=rNode;
	ui->setupUi(this);
	//ui->treeWidget->setRootIsDecorated(true);
	setupNode();
}
AnimationFrameEditor::~AnimationFrameEditor()
{
	delete ui;
}

void AnimationFrameEditor::setupNode() {
	if (mNode) {
		int i=0;
		string currentResourceName="";

		if (mNode->getNodeType()==NodeType::Animation) {
			NodeAnimation *rNodeAnimation=static_cast<NodeAnimation*>(mNode);
			ui->animationName->setText(QString::fromStdString(rNodeAnimation->getName()));
		}

		if (GuiContext::getInstance().getCurrentResource()) {
			currentResourceName=GuiContext::getInstance().getCurrentResource()->getName();
		}

		vector<string> v=ProjectContext::getInstance().getFileNamesForType(NodeInfoType::Resource);
		for (auto& s : v) {
			ui->resourceNameOpm->addItem(QString::fromStdString(s));
			if (s==currentResourceName) {
				ui->resourceNameOpm->setCurrentIndex(i);
			}
			i++;
		}
		vector<Node*> vAnimationFrames=mNode->getChildNodesWithNodeType(NodeType::AnimationFrame);
		for (auto rNode : vAnimationFrames) {
			NodeAnimationFrame* rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNode);
			mAnimationFrameEntryList.emplace_back(rNodeAnimationFrame->getFrameRef().resourcefile, rNodeAnimationFrame->getFrameRef().textureid, rNodeAnimationFrame->getFrameRef().frame );
		}
		buildAnimationFrameView();
	}
}

void AnimationFrameEditor::buildAnimationFrameView() {
	ui->treeWidgetAnimation->clear();
	int i=0;
	for(AnimationFrameEntry&e : mAnimationFrameEntryList) {
		NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(e.resourcefile);
		if (rNodeResource) {
			Node* rNodeT=rNodeResource->getNodeWithNodeId(e.textureid);
			if (rNodeT && rNodeT->getNodeType()==NodeType::Texture) {
				NodeTexture *rNodeTexture=static_cast<NodeTexture*>(rNodeT);
				NodeTextureFrame *rNodeTextureFrame=nullptr;
				if (!e.frame.empty()) {
					Node* rNodeF=rNodeTexture->getChildNodeWithNameAndNodeType(e.frame, NodeType::TextureFrame);
					if (rNodeF) {
						rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeF);
					}
				}
				QTreeWidgetItem *r=new QTreeWidgetItem(ui->treeWidgetAnimation);
				BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
				if (bTexture) {
					QLabel *rPreviewImage=new QLabel(QString::fromStdString(to_string(i)));
					QImage img(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
					if (rNodeTextureFrame) {
						QImage imgFrame=img.copy(rNodeTextureFrame->getFrame().x,
												 rNodeTextureFrame->getFrame().y,
												 rNodeTextureFrame->getFrame().width,
												 rNodeTextureFrame->getFrame().height);
						QImage scaled = imgFrame.scaled(30, 30, Qt::AspectRatioMode::KeepAspectRatio); // Scale image to show results better
						QPixmap rQPixmap = QPixmap::fromImage(scaled); // Create pixmap from image
						rPreviewImage->setPixmap(rQPixmap);
					} else {
						QImage scaled = img.scaled(30, 30, Qt::AspectRatioMode::KeepAspectRatio); // Scale image to show results better
						QPixmap rQPixmap = QPixmap::fromImage(scaled); // Create pixmap from image
						rPreviewImage->setPixmap(rQPixmap);
					}
					ui->treeWidgetAnimation->setItemWidget(r,0, rPreviewImage);
				}
				//r->setText(0, QString::fromStdString(to_string(i)));
				// Texture Name
				r->setText(1, QString::fromStdString(rNodeTexture->getName()));
				// Texture Frame Name
				r->setText(2, QString::fromStdString(e.frame));
				// Texture Path
				r->setText(3, QString::fromStdString(rNodeTexture->getPath()));
				// Resource File
				r->setText(4, QString::fromStdString(e.resourcefile));
				ui->treeWidgetAnimation->addTopLevelItem(r);
			}
		}
		i++;
	}
}

void AnimationFrameEditor::setupCurrentResource(const string &rName) {
	ui->treeWidgetTextures->clear();
	NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(rName);
	if (rNodeResource) {
		int cnt=rNodeResource->getChildCount();
		for (int i=0;i<cnt;i++) {
			Node *rNode=rNodeResource->getNodeFromIndex(i);
			if (rNode && rNode->getNodeType()==NodeType::Texture) {
				NodeTexture* rNodeTexture=static_cast<NodeTexture*>(rNode);
				QTreeWidgetItem *r=new QTreeWidgetItem(ui->treeWidgetTextures);
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
					ui->treeWidgetTextures->setItemWidget(r,1, rPreviewImage);
					//TreeUtil::setNodeToTreeItem(r, rNode);
					ui->treeWidgetTextures->addTopLevelItem(r);
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
								ui->treeWidgetTextures->setItemWidget(rc,1, rPreviewImageFrame);
								r->addChild(rc);
							}
						}
					}
				}
			}
		}
	}
}

void AnimationFrameEditor::on_okButton_clicked()
{
	int i=0;
	mNode->deleteChildNodes();
	mNode->setName(ui->animationName->text().toStdString());
	NodeType rNodeType=NodeType::AnimationFrame;
	for(auto&rAnimationFrameEntry : mAnimationFrameEntryList) {
		Node*rNode=getInstanceFromNodeType(rNodeType, true);
		if (rNode) {
			NodeAnimationFrame *rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNode);
			rNodeAnimationFrame->setName(rAnimationFrameEntry.frame);
			rNodeAnimationFrame->getFrameRef().frame=rAnimationFrameEntry.frame;
			rNodeAnimationFrame->getFrameRef().textureid=rAnimationFrameEntry.textureid;
			rNodeAnimationFrame->getFrameRef().resourcefile=rAnimationFrameEntry.resourcefile;
			mNode->addChildNode(rNodeAnimationFrame);
		}
		i++;
	}
	GuiContext::getInstance().updateNodeName(mNode, NodeInfoType::Resource);
	GuiContext::getInstance().updateChildNodes(mNode, NodeInfoType::Resource);
	deleteLater();
}


void AnimationFrameEditor::on_cancelButton_clicked()
{
	deleteLater();
}

void AnimationFrameEditor::on_AnimationFrameEditor_rejected()
{
	deleteLater();
}

void AnimationFrameEditor::on_resourceNameOpm_currentIndexChanged(int index)
{
	QString rCurrentResource=ui->resourceNameOpm->currentText();
	setupCurrentResource(rCurrentResource.toStdString());
}


void AnimationFrameEditor::on_treeWidgetTextures_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	addSelectedFrames(false);
}

void AnimationFrameEditor::on_addFrame_clicked() {
	addSelectedFrames(true);
}

void AnimationFrameEditor::addSelectedFrames(bool setNextSelection)
{
	QList<QTreeWidgetItem*>selectedItems=ui->treeWidgetTextures->selectedItems();
	if (selectedItems.size()>0) {
		QTreeWidgetItem* itemLast=nullptr;
		for (QTreeWidgetItem* r : selectedItems) {
			itemLast=r;
			Node*rNode=TreeUtil::getNodeFromTreeItem(r);
			if (rNode) {
				if (rNode->getNodeType()==NodeType::TextureFrame) {
					NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
					mAnimationFrameEntryList.emplace_back(ui->resourceNameOpm->currentText().toStdString(), rNode->getParent()->getId(), rNodeTextureFrame->getName());
				} else if (rNode->getNodeType()==NodeType::Texture) {
					NodeTexture *rNodeTexture=static_cast<NodeTexture*>(rNode);
					mAnimationFrameEntryList.emplace_back(ui->resourceNameOpm->currentText().toStdString(), rNodeTexture->getId(), "");
				}
			}
		}
		if (setNextSelection) {
			QModelIndexList indexes = ui->treeWidgetTextures->selectionModel()->selectedRows();
			ui->treeWidgetTextures->clearSelection();
			if (itemLast->parent()) {
				if (itemLast->parent()->childCount()-1>indexes.last().row()) {
					itemLast->parent()->child(indexes.last().row()+1)->setSelected(true);
				}
			} else {
				if (ui->treeWidgetTextures->topLevelItemCount()-1>indexes.last().row()) {
					ui->treeWidgetTextures->topLevelItem(indexes.last().row()+1)->setSelected(true);
				}
			}
		}

		buildAnimationFrameView();
	}
}

void AnimationFrameEditor::on_removeFrame_clicked()
{
	int firstSelectedRow=-1;
	QModelIndexList indexes = ui->treeWidgetAnimation->selectionModel()->selectedRows();
	if (indexes.size()>0) {
		firstSelectedRow=indexes.at(0).row();
		for (int i=indexes.size()-1;i>=0;i--) {
			mAnimationFrameEntryList.erase(mAnimationFrameEntryList.begin()+indexes.at(i).row());
		}
		mAnimationFrameEntryList.shrink_to_fit();
		buildAnimationFrameView();
		if (firstSelectedRow>=0) {
			if (ui->treeWidgetAnimation->topLevelItemCount()>firstSelectedRow) {
				ui->treeWidgetAnimation->topLevelItem(firstSelectedRow)->setSelected(true);
			} else if (ui->treeWidgetAnimation->topLevelItemCount()>0) {
				ui->treeWidgetAnimation->topLevelItem(ui->treeWidgetAnimation->topLevelItemCount()-1)->setSelected(true);
			}
		}
	}
}

