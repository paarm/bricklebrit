#include "textureframeeditor.h"
#include "ui_textureframeeditor.h"
#include "../guicontext.h"

TextureFrameEditor::TextureFrameEditor(Node *rNode, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TextureFrameEditor)
{
	mNode=rNode;
	ui->setupUi(this);
	//ui->treeWidget->setRootIsDecorated(true);
	setupNode();
}

TextureFrameEditor::~TextureFrameEditor()
{
	//std::cout << "TextureFrameEditor destoryed" << std::endl;
	delete ui;
}

void TextureFrameEditor::setupNode() {
	if (mNode) {
		if (mNode->getNodeType()==NodeType::Texture) {
			NodeTexture *rNodeTexture=static_cast<NodeTexture*>(mNode);
			ui->textureName->setText(QString::fromStdString(rNodeTexture->getPath()));
			if (mImage.load(QString::fromStdString(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), rNodeTexture->getPath())))) {
				mImageLoaded=true;
				mImageWidth=mImage.width();
				mImageHeight=mImage.height();
				mImageScaled=mImage.scaled(QSize(100,100),
							  Qt::KeepAspectRatio);
				ui->imageWidth->setText(QString::number(mImageWidth));
				ui->imageHeight->setText(QString::number(mImageHeight));
				ui->widhtFrame->setValue(mImageWidth);
				ui->heightFrame->setValue(mImageHeight);
				ui->previewImage->setPixmap(mImageScaled);
			}
			int childCnt=mNode->getChildCount();
			if (childCnt>0) {
				for (int i=0;i<childCnt;i++) {
					Node* rNodeChild=mNode->getNodeFromIndex(i);
					if (rNodeChild->getNodeType()==NodeType::TextureFrame) {
						NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeChild);
						createFrame(rNodeTextureFrame->getName(), rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y,
									rNodeTextureFrame->getFrame().width,rNodeTextureFrame->getFrame().height);
					}
				}
				updateFrameView();
			}
		}
	}
}

void TextureFrameEditor::on_cancelButton_clicked()
{
	deleteLater();
}

void TextureFrameEditor::on_TextureFrameEditor_rejected()
{
	deleteLater();
}

void TextureFrameEditor::on_okButton_clicked()
{
	int i=0;
	mNode->deleteChildNodes();
	NodeType rNodeType=NodeType::TextureFrame;
	for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
		Node*rNode=getInstanceFromNodeType(rNodeType, true);
		if (rNode) {
			NodeTextureFrame *rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNode);
			rNodeTextureFrame->setName(rTextureFrameEntry.name);
			rNodeTextureFrame->getFrame().x=rTextureFrameEntry.x;
			rNodeTextureFrame->getFrame().y=rTextureFrameEntry.y;
			rNodeTextureFrame->getFrame().width=rTextureFrameEntry.w;
			rNodeTextureFrame->getFrame().height=rTextureFrameEntry.h;
			mNode->addChildNode(rNodeTextureFrame);
		}
		i++;
	}
	GuiContext::getInstance().updateChildNodes(mNode, NodeInfoType::Resource);
	deleteLater();
}

void TextureFrameEditor::on_addFrames_clicked()
{
	int w=0;
	int h=0;
	if (ui->xFrames->value()>0) {
		w=mImageWidth/ui->xFrames->value();
	}
	if (ui->yFrames->value()>0) {
		h=mImageHeight/ui->yFrames->value();
	}
	if (w>0 && h>0) {
		int cntx=mImageWidth/w;
		int cnty=mImageHeight/h;
		for (int iy=0;iy<cnty;iy++) {
			for (int ix=0;ix<cntx;ix++) {
				createFrame("", ix*w, iy*h, w, h);
			}
		}
		updateFrameView();
	}
}

void TextureFrameEditor::createFrame(const string&rName, int x, int y, int w, int h) {
	for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
		if (rTextureFrameEntry.equal(x,y,w,h)) {
			return;
		}
	}
	string name=rName;
	if (name.size()==0) {
		int name_bn=mTextureFrameEntryList.size();
		name=to_string(mTextureFrameEntryList.size());
		while(true) {
			int found=false;
			for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
				if (rTextureFrameEntry.name==name) {
					found=true;
					break;
				}
			}
			if (found) {
				name_bn++;
				name=to_string(name_bn);
			} else {
				break;
			}
		}
	}
	mTextureFrameEntryList.emplace_back(name,x,y,w,h);
}

void TextureFrameEditor::updateFrameView() {
	ui->treeWidget->clear();
	int i=0;
	for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
		QTreeWidgetItem *r=new QTreeWidgetItem();
		r->setText(0, QString::fromStdString(rTextureFrameEntry.name));
		r->setText(1, QString::number(rTextureFrameEntry.x));
		r->setText(2, QString::number(rTextureFrameEntry.y));
		r->setText(3, QString::number(rTextureFrameEntry.w));
		r->setText(4, QString::number(rTextureFrameEntry.h));
		ui->treeWidget->addTopLevelItem(r);
		i++;
	}
}

void TextureFrameEditor::on_addSingleFrame_clicked()
{
	int x=max(0,ui->xFrame->value());
	int y=max(0,ui->yFrame->value());
	int w=max(0,ui->widhtFrame->value());
	int h=max(0,ui->heightFrame->value());

	x=min(mImageWidth, x);
	y=min(mImageHeight, y);

	w=min(w, mImageWidth-x);
	h=min(h, mImageHeight-y);

	createFrame("", x, y, w, h);
	updateFrameView();
}

int TextureFrameEditor::getTextureFrameEntryIndexFromTreeWidgetItem(QTreeWidgetItem* rTreeWidgetItem) {
	int idx=0;
	int rv=-1;
	for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
		if (rTextureFrameEntry.name==rTreeWidgetItem->text(0).toStdString()) {
			rv=idx;
			break;
		}
		idx++;
	}
	return rv;
}

void TextureFrameEditor::on_removeFrame_clicked()
{
	QList<QTreeWidgetItem*>selectedItems=ui->treeWidget->selectedItems();
	if (selectedItems.count()>0) {
		int firstSelected=-1;
		for (int i=selectedItems.count()-1;i>=0;i--) {
			QTreeWidgetItem *r=selectedItems.at(i);
			int idx=0;
			int frame=-1;
			for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
				if (rTextureFrameEntry.name==r->text(0).toStdString()) {
					frame=idx;
					break;
				}
				idx++;
			}
			if (frame>=0) {
				if (firstSelected==-1) {
					firstSelected=frame;
				}
			}
			mTextureFrameEntryList.erase(mTextureFrameEntryList.begin()+frame);
		}
		mTextureFrameEntryList.shrink_to_fit();
		updateFrameView();

		int selectThisIndex=-1;
		if (firstSelected>=0 && ui->treeWidget->topLevelItemCount()>firstSelected) {
			selectThisIndex=firstSelected;
		} else if (ui->treeWidget->topLevelItemCount()>0) {
			selectThisIndex=ui->treeWidget->topLevelItemCount()-1;
		}
		if (selectThisIndex>=0) {
			ui->treeWidget->topLevelItem(selectThisIndex)->setSelected(true);
		}
	}
}

void TextureFrameEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if (mImageLoaded && item && column>=0) {
		int rIndex=getTextureFrameEntryIndexFromTreeWidgetItem(item);
		if (rIndex>=0) {
			TextureFrameEntry &rTextureFrameEntry=mTextureFrameEntryList.at(rIndex);
			mImageFrame=mImage.copy(rTextureFrameEntry.x, rTextureFrameEntry.y, rTextureFrameEntry.w, rTextureFrameEntry.h);
			mImageFrame=mImageFrame.scaled(QSize(100,100), Qt::KeepAspectRatio);
			ui->previewFrame->setPixmap(mImageFrame);
		}
	}
}

/*void TextureFrameEditor::on_treeWidget_itemEntered(QTreeWidgetItem *item, int column)
{
}

void TextureFrameEditor::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
}*/
