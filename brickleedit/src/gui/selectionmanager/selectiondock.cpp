#include "selectiondock.h"
#include "ui_selectiondock.h"
#include "../treeutil.h"
#include <QPushButton>
#include "../spriteeditor/spriteeditor.h"
#include "../guicontext.h"

SelectionDock::SelectionDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SelectionDock)
{
	ui->setupUi(this);
	ui->deselectOther->setEnabled(false);
	ui->setAsCanvas->setEnabled(false);
	ui->treeWidget->setColumnWidth(0, 50);
	ui->treeWidget->setColumnWidth(1, 100);
	ui->treeWidget->setColumnWidth(2, 100);
	ui->treeWidget->setColumnWidth(3, 50);
}

SelectionDock::~SelectionDock()
{
	delete ui;
}

void SelectionDock::removeNode(Node *rNode) {
	if (rNode) {
		QTreeWidgetItem *r=TreeUtil::getTreeWidgetItemFromNode(ui->treeWidget, rNode);
		if (r) {
			delete r;
		}
		updateLatestSelected();
	}
}

void SelectionDock::removeAllNodes() {
	ui->treeWidget->clear();
	ui->deselectOther->setEnabled(false);
}

void SelectionDock::addNode(Node *rNode) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
		r=new QTreeWidgetItem(ui->treeWidget);
		// Type + ID
		TreeUtil::setTypeNameToTreeItem(1, r, rNode);
		// Name
		TreeUtil::setNodeNameToTreeItem(2, r, rNode);
		//r->setText(1,QString::fromStdString(rNode->getName()));
		TreeUtil::setNodeDataToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		//ui->treeWidget->addTopLevelItem(r);

		QPushButton *rButtonDeselect=new QPushButton("X",ui->treeWidget);
		rButtonDeselect->setToolTip(QObject::tr("Deselect"));
		//rButtonDeselect->setFixedWidth(rButtonDeselect->fontMetrics().width(" ... "));
		connect(rButtonDeselect, &QPushButton::clicked, this, [rButtonDeselect, rNode, this]() {
			GuiContext::getInstance().getSelectionManager().deselectNode(rNode);
			GuiContext::getInstance().updateGlWidget();
		});
		ui->treeWidget->setItemWidget(r, 0, rButtonDeselect);

		if (rNode->getNodeType()==NodeType::Sprite) {
			QPushButton *rButton=new QPushButton("S",ui->treeWidget);
			rButton->setToolTip(QObject::tr("Edit Sprite..."));
			//rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
			connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
				SpriteEditor *rSpriteEditor=new SpriteEditor(rNode, ui->treeWidget);
				rSpriteEditor->show();
			});
			ui->treeWidget->setItemWidget(r, 3, rButton);
		}
		updateLatestSelected();
	}
}

void SelectionDock::updateLatestSelected() {
	Node *rLatestNode=GuiContext::getInstance().getSelectionManager().getLatestSelected();
	if (rLatestNode) {
		QTreeWidgetItem *r=TreeUtil::getTreeWidgetItemFromNode(ui->treeWidget, rLatestNode);
		if (r && !r->isSelected()) {
			ui->treeWidget->clearSelection();
			r->setSelected(true);
			ui->treeWidget->scrollToItem(r);
		}
	}
	if (ui->treeWidget->topLevelItemCount()>1) {
		ui->deselectOther->setEnabled(true);
	} else {
		ui->deselectOther->setEnabled(false);
	}
	if (ui->treeWidget->topLevelItemCount()>0) {
		ui->setAsCanvas->setEnabled(true);
	} else {
		ui->setAsCanvas->setEnabled(false);
	}
}

void SelectionDock::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column) {
	ignoreparam(column);
	ignoreparam(item);
}

void SelectionDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	ignoreparam(column);
	Node *rNode=TreeUtil::getNodeFromTreeItem(item);
	if (rNode) {
		if (rNode!=GuiContext::getInstance().getSelectionManager().getLatestSelected()) {
			GuiContext::getInstance().getSelectionManager().setAsLatestSelected(rNode);
			GuiContext::getInstance().updateGlWidget();
		}
	}
}


void SelectionDock::on_deselectOther_clicked()
{
	if (GuiContext::getInstance().getSelectionManager().getLatestSelected()) {
		GuiContext::getInstance().getSelectionManager().replaceAllSelectedWithNode(GuiContext::getInstance().getSelectionManager().getLatestSelected());
		GuiContext::getInstance().updateGlWidget();
	}
}

void SelectionDock::on_setAsCanvas_clicked()
{
	Node *rNode=GuiContext::getInstance().getSelectionManager().getLatestSelected();
	if (rNode &&
			(rNode->getNodeType()==NodeType::Scene || rNode->getNodeType()==NodeType::Sprite)
			) {
		GuiContext::getInstance().setCurrentPaintCanvas(static_cast<Node2d*>(rNode), true);
	}
}
