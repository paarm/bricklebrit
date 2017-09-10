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
		NodeResource* rNodeResource=ProjectContext::getInstance().getNodeResourceByName(e.resourcefile);
		if (rNodeResource) {
			Node* rNodeT=rNodeResource->getNodeWithNodeId(e.textureid);
			if (rNodeT && rNodeT->getNodeType()==NodeType::Texture) {
				NodeTexture *rNodeTexture=static_cast<NodeTexture*>(rNodeT);
				QTreeWidgetItem *r=new QTreeWidgetItem(ui->treeWidgetAnimation);
				r->setText(0, QString::fromStdString(to_string(i)));
				r->setText(1, QString::fromStdString("texture: "+rNodeTexture->getName()+"("+to_string(e.textureid)+")"+"|frame: "+e.frame+"| resource file: "+rNodeTexture->getPath()+"|"));
				ui->treeWidgetAnimation->addTopLevelItem(r);
			}
		}
		i++;
	}
}

void AnimationFrameEditor::setupCurrentResource(const string &rName) {
	ui->treeWidgetTextures->clear();
	NodeResource* rNodeResource=ProjectContext::getInstance().getNodeResourceByName(rName);
	if (rNodeResource) {
		int cnt=rNodeResource->getChildCount();
		for (int i=0;i<cnt;i++) {
			Node *rNode=rNodeResource->getNodeFromIndex(i);
			if (rNode && rNode->getNodeType()==NodeType::Texture) {
				QTreeWidgetItem *r=new QTreeWidgetItem(ui->treeWidgetTextures);
				TreeUtil::setNodeToTreeItem(r, rNode);
				ui->treeWidgetTextures->addTopLevelItem(r);
				r->setExpanded(true);

				if (rNode->getChildCount()>0) {
					unsigned long countChild=rNode->getChildCount();
					for (unsigned long i=0;i<countChild;i++) {
						Node *rNodeChild=rNode->getNodeFromIndex(i);
						if (rNodeChild && rNodeChild->getNodeType()==NodeType::TextureFrame) {
							QTreeWidgetItem *rc=new QTreeWidgetItem(r);
							TreeUtil::setNodeToTreeItem(rc, rNodeChild);
							r->addChild(rc);
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

void AnimationFrameEditor::on_addFrame_clicked()
{
	QList<QTreeWidgetItem*>selectedItems=ui->treeWidgetTextures->selectedItems();
	if (selectedItems.size()>0) {
		for (QTreeWidgetItem* r : selectedItems) {
			Node*rNode=TreeUtil::getNodeFromTreeItem(r);
			if (rNode) {
				if (rNode->getNodeType()==NodeType::TextureFrame) {
					NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
					mAnimationFrameEntryList.emplace_back(ui->resourceNameOpm->currentText().toStdString(), rNode->getParent()->getId(), rNodeTextureFrame->getName());
				}
			}
		}
		buildAnimationFrameView();
	}
}
