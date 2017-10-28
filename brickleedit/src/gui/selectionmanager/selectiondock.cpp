#include "selectiondock.h"
#include "ui_selectiondock.h"
#include "../treeutil.h"
#include "../spriteeditor/spriteeditor.h"
#include "../guicontext.h"
#include <QList>

SelectionDock::SelectionDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SelectionDock)
{
	ui->setupUi(this);
	ui->deselectOther->setEnabled(false);
	ui->setAsCanvas->setEnabled(false);
    ui->treeWidget->setColumnWidth(0, 80);
	ui->treeWidget->setColumnWidth(1, 100);
	ui->treeWidget->setColumnWidth(2, 100);
    //ui->treeWidget->setColumnWidth(3, 50);
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

void SelectionDock::addNodes(vector<Node*> &v) {
    if (!v.empty()) {
        QList<QTreeWidgetItem*> vv;
        for(Node* rNode : v) {
            QTreeWidgetItem*r=prepareNodeIntern(rNode);
            if (r) {
                vv.append(r);
            }
        }
        ui->treeWidget->addTopLevelItems(vv);
        int i=0;
        for(Node* rNode : v) {
            QTreeWidgetItem*r=vv.at(i);
            QPushButton* rDeselectButton=prepareDeselectButton(rNode);
            QPushButton* rEditSpriteButton=prepareEditSpriteButton(rNode);

            QWidget *dualPushButtons = new QWidget();
            QHBoxLayout *hLayout = new QHBoxLayout();
            hLayout->setContentsMargins(0,0,0,0);

            if (rDeselectButton) {
                hLayout->addWidget(rDeselectButton);
            }
            if (rEditSpriteButton) {
                hLayout->addWidget(rEditSpriteButton);
            }
            dualPushButtons->setLayout(hLayout);
            ui->treeWidget->setItemWidget(r, 0, dualPushButtons);

            i++;
        }
        updateLatestSelected();
    }
}

void SelectionDock::addNode(Node *rNode) {
    QTreeWidgetItem*r=prepareNodeIntern(rNode);
    if (r) {
        ui->treeWidget->addTopLevelItem(r);
        QPushButton* rDeselectButton=prepareDeselectButton(rNode);
        QPushButton* rEditSpriteButton=prepareEditSpriteButton(rNode);

        QWidget *dualPushButtons = new QWidget();
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->setContentsMargins(0,0,0,0);

        if (rDeselectButton) {
            hLayout->addWidget(rDeselectButton);
        }
        if (rEditSpriteButton) {
            hLayout->addWidget(rEditSpriteButton);
        }
        dualPushButtons->setLayout(hLayout);
        ui->treeWidget->setItemWidget(r, 0, dualPushButtons);

        updateLatestSelected();
    }
}

QPushButton* SelectionDock::prepareDeselectButton(Node *rNode) {
    QPushButton *rButtonDeselect=new QPushButton("X",ui->treeWidget);
    rButtonDeselect->setToolTip(QObject::tr("Deselect"));
    rButtonDeselect->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

    //rButtonDeselect->setSizePolicy(QSizePolicy::Maximum);
    //rButtonDeselect->setFixedWidth(rButtonDeselect->fontMetrics().width(" ... "));
    connect(rButtonDeselect, &QPushButton::clicked, this, [rButtonDeselect, rNode, this]() {
        GuiContext::getInstance().getSelectionManager().deselectNode(rNode);
        GuiContext::getInstance().updateGlWidget();
    });
    return rButtonDeselect;
}

QPushButton* SelectionDock::prepareEditSpriteButton(Node *rNode) {
    QPushButton *rButton=new QPushButton("S",ui->treeWidget);
    rButton->setToolTip(QObject::tr("Edit Sprite..."));
    rButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    //rButton->setSizePolicy(QSizePolicy::Maximum);//rButton->setFixedWidth(rButton->fontMetrics().width(" ... "));
    connect(rButton, &QPushButton::clicked, this, [rButton, rNode, this]() {
        SpriteEditor *rSpriteEditor=new SpriteEditor(rNode, ui->treeWidget);
        rSpriteEditor->show();
    });
    return rButton;
}

QTreeWidgetItem*  SelectionDock::prepareNodeIntern(Node *rNode) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
        r=new QTreeWidgetItem();
		// Type + ID
		TreeUtil::setTypeNameToTreeItem(1, r, rNode);
		// Name
		TreeUtil::setNodeNameToTreeItem(2, r, rNode);
		//r->setText(1,QString::fromStdString(rNode->getName()));
		TreeUtil::setNodeDataToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		//ui->treeWidget->addTopLevelItem(r);
	}
    return r;
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
