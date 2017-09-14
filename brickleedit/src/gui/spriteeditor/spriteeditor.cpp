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
	mNode=rNode;
	setupNode();
}


SpriteEditor::~SpriteEditor()
{
	delete ui;
}

void SpriteEditor::setupNode() {
	string currentResourceName="";
	if (GuiContext::getInstance().getCurrentResource()) {
		currentResourceName=GuiContext::getInstance().getCurrentResource()->getName();
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
}

void SpriteEditor::on_okButton_clicked()
{
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
