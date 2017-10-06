#include "textureframeeditor.h"
#include "ui_textureframeeditor.h"
#include "../guicontext.h"
#include <QFileDialog>

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
            setupTexture(rNodeTexture->getPath());
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
				//updateFrameView();
			}
		}
	}
}

void TextureFrameEditor::setupTexture(const string &rPath) {
    ui->textureName->setText(QString::fromStdString(rPath));
    if (mImage.load(QString::fromStdString(DirUtil::concatPath(ProjectContext::getInstance().getProjectPathAbs(), rPath)))) {
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
    } else {
        mImageLoaded=false;
    }
}

void TextureFrameEditor::on_pushButton_clicked()
{
    QString file=QFileDialog::getOpenFileName(this, tr("Select Texture"),
                                              QString::fromStdString(ProjectContext::getInstance().getProjectPathAbs()),
                                              "*.png",
                                              nullptr,
                                              QFileDialog::DontResolveSymlinks
                                              );
    string rName=DirUtil::subPart1FromPart2(ProjectContext::getInstance().getProjectPathAbs(), file.toStdString());
    if (!rName.empty() && rName!=ui->textureName->text().toStdString()) {
        setupTexture(rName);
        mTextureFrameEntryList.clear();
		ui->treeWidget->clear();
		//updateFrameView();
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
	bool isNewNode=true;
	if (mNode) {
		isNewNode=false;
		mNode->deleteChildNodes();
	} else {
		mNode=getInstanceFromNodeType(NodeType::Texture, true);
	}
	if (mNode) {
		NodeTexture* rNodeTexture=static_cast<NodeTexture*>(mNode);
		rNodeTexture->setPath(ui->textureName->text().toStdString());
		rNodeTexture->setName(ui->textureName->text().toStdString());

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
		if (isNewNode) {
			GuiContext::getInstance().insertNewResourceNode(mNode);
		} else {
			GuiContext::getInstance().updateChildNodes(mNode);
			GuiContext::getInstance().updateNodeName(mNode);
		}
	}
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
		//updateFrameView();
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
	QTreeWidgetItem *r=new QTreeWidgetItem();
	ui->treeWidget->addTopLevelItem(r);
	addContentForFrameItem(r, mTextureFrameEntryList.back());
}

void TextureFrameEditor::addContentForFrameItem(QTreeWidgetItem *r, TextureFrameEntry& rTextureFrameEntry) {
	QLabel *rPreviewImage=new QLabel(QString::fromStdString(rTextureFrameEntry.name));
	QPixmap imgFrame=mImage.copy(rTextureFrameEntry.x, rTextureFrameEntry.y, rTextureFrameEntry.w, rTextureFrameEntry.h);
	imgFrame=imgFrame.scaled(QSize(30,30), Qt::KeepAspectRatio);
	//QPixmap rQPixmap = QPixmap::fromImage(imgFrame); // Create pixmap from image
	rPreviewImage->setPixmap(imgFrame);
	ui->treeWidget->setItemWidget(r,0, rPreviewImage);

	//r->setText(0, QString::fromStdString(rTextureFrameEntry.name));
	r->setText(1, QString::number(rTextureFrameEntry.x));
	r->setText(2, QString::number(rTextureFrameEntry.y));
	r->setText(3, QString::number(rTextureFrameEntry.w));
	r->setText(4, QString::number(rTextureFrameEntry.h));
}

#if 0
void TextureFrameEditor::updateFrameView() {
	ui->treeWidget->clear();
	int i=0;
	for(auto&rTextureFrameEntry : mTextureFrameEntryList) {
		QTreeWidgetItem *r=new QTreeWidgetItem();
		ui->treeWidget->addTopLevelItem(r);
		addContentForFrameItem(r, rTextureFrameEntry);
		i++;
	}
}
#endif
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
	//updateFrameView();
}

void TextureFrameEditor::on_removeFrame_clicked()
{
	int firstSelectedRow=-1;
	QModelIndexList indexes = ui->treeWidget->selectionModel()->selectedRows();
	if (indexes.size()>0) {
		firstSelectedRow=indexes.at(0).row();
		for (int i=indexes.size()-1;i>=0;i--) {
			mTextureFrameEntryList.erase(mTextureFrameEntryList.begin()+indexes.at(i).row());
			ui->treeWidget->takeTopLevelItem(indexes.at(i).row());
		}
		mTextureFrameEntryList.shrink_to_fit();
		//buildAnimationFrameView();
		if (firstSelectedRow>=0) {
			if (ui->treeWidget->topLevelItemCount()>firstSelectedRow) {
				ui->treeWidget->topLevelItem(firstSelectedRow)->setSelected(true);
				on_treeWidget_itemClicked(ui->treeWidget->topLevelItem(firstSelectedRow),0);
			} else if (ui->treeWidget->topLevelItemCount()>0) {
				ui->treeWidget->topLevelItem(ui->treeWidget->topLevelItemCount()-1)->setSelected(true);
				on_treeWidget_itemClicked(ui->treeWidget->topLevelItem(ui->treeWidget->topLevelItemCount()-1),0);
			} else {
				ui->previewFrame->setPixmap(QPixmap());
			}
		}
	}
}

void TextureFrameEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if (mImageLoaded && item && column>=0) {
		int rIndex=ui->treeWidget->indexOfTopLevelItem(item);
		if (rIndex>=0 && static_cast<int>(mTextureFrameEntryList.size())>rIndex) {
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

