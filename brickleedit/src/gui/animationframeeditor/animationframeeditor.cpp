#include "animationframeeditor.h"
#include "ui_animationframeeditor.h"
#include "../../project/projectcontext.h"

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
		vector<string> v=ProjectContext::getInstance().getFileNamesForType(NodeInfoType::Resource);
		for (auto& s : v) {
			ui->resourceNameOpm->addItem(QString::fromStdString(s));
		}
		if (v.size()>0) {
			ui->resourceNameOpm->setCurrentIndex(0);
		}
		if (mNode->getNodeType()==NodeType::Animation) {
			NodeAnimation *rNodeAnimation=static_cast<NodeAnimation*>(mNode);
			ui->animationName->setText(QString::fromStdString(rNodeAnimation->getName()));
		}
	}
}

void AnimationFrameEditor::on_okButton_clicked()
{
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
