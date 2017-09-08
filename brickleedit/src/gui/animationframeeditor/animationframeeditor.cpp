#include "animationframeeditor.h"
#include "ui_animationframeeditor.h"

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
