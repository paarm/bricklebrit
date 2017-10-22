#include "scenetreedock.h"
#include "ui_scenetreedock.h"
#include "../guicontext.h"
#include "../textureframeeditor/textureframeeditor.h"
#include "../animationframeeditor/animationframeeditor.h"
#include "../spriteeditor/spriteeditor.h"
#include "../treeutil.h"

SceneTreeDock::SceneTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SceneTreeDock)
{
	ui->setupUi(this);

	ui->treeWidget->setColumnWidth(0,170);
	ui->treeWidget->setColumnWidth(1,50);

	ui->treeWidgetResources->setColumnWidth(0,170);
	ui->treeWidgetResources->setColumnWidth(2,50);

	ui->sceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());
	ui->resourceMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewResource());

	mTabInfoScene.setQTreeWidget(ui->treeWidget);
	mTabInfoResource.setQTreeWidget(ui->treeWidgetResources);

	setSceneEditable(false);
	setResourceEditable(false);
	setProjectAvailable(false);
}

SceneTreeDock::~SceneTreeDock()
{
	delete ui;
}

void SceneTreeDock::clearScene() {
	mTabInfoScene.clear();
	setSceneEditable(false);
}

void SceneTreeDock::clearSceneOpm() {
	ui->sceneNameOpm->clear();
}

void SceneTreeDock::clearResource() {
	mTabInfoResource.clear();
	setResourceEditable(false);
}

void SceneTreeDock::clearResourceOpm() {
	ui->resourceNameOpm->clear();
}


void SceneTreeDock::setResourceEditable(bool isEditable) {
	ui->newTexture->setEnabled(isEditable);
	ui->newAnimation->setEnabled(isEditable);
}

void SceneTreeDock::setProjectAvailable(bool isActive) {
	ui->sceneMenu->setEnabled(isActive);
	ui->resourceMenu->setEnabled(isActive);
	if (!isActive) {
		clearResourceOpm();
		clearSceneOpm();
	}
}


int SceneTreeDock::getActiveTreeTabIndex() {
	return ui->tabWidget->currentIndex();
}

void SceneTreeDock::addLayerNode(NodeLayer *rNode) {
	QTreeWidgetItem* r=nullptr;
	if (rNode) {
		r=new QTreeWidgetItem(ui->treeWidget);
		// Name
		TreeUtil::setNodeNameToTreeItem(0, r, rNode);
		TreeUtil::setNodeDataToTreeItem(r,rNode);
		//r->setIcon(0, QIcon(":/icons/new.png"));
		ui->treeWidget->addTopLevelItem(r);

		QIcon my_icon;
		my_icon.addFile(":/icons/eyeOpen.png", QSize(), QIcon::Normal, QIcon::On);
		my_icon.addFile(":/icons/delete.png", QSize(), QIcon::Normal, QIcon::Off);
		QPushButton *rButtonVisible=new QPushButton(my_icon, "",ui->treeWidget);
		rButtonVisible->setFocusPolicy(Qt::NoFocus);
		//rButtonVisible->setToolTip(QObject::tr("Visible"));
		//rButtonVisible->setIcon(QIcon(":/icons/eyeOpen.png"));
		//rButtonVisible->setFixedWidth(rButtonVisible->fontMetrics().width(" ... "));
		rButtonVisible->setCheckable(true);
		connect(rButtonVisible, &QPushButton::toggled, this, [rButtonVisible, rNode, this]() {
			GuiContext::getInstance().getLayerManager().setLayerVisible(rNode, rButtonVisible->isChecked());
			GuiContext::getInstance().updateNodeName(rNode);
			if (!rButtonVisible->isChecked()) {
				GuiContext::getInstance().getSelectionManager().deselectAllNodes();
			}
			GuiContext::getInstance().updateGlWidget();
		});
		rButtonVisible->setChecked(rNode->getVisible());
		ui->treeWidget->setItemWidget(r, 1, rButtonVisible);
		setCurrentLayerAsSelected();
	}
}

void SceneTreeDock::updateLayerName(NodeLayer *rNode) {
	QTreeWidgetItem* r=TreeUtil::getTreeWidgetItemFromNode(ui->treeWidget, rNode);
	if (r) {
		TreeUtil::setNodeNameToTreeItem(0, r, rNode);
	}
}

void SceneTreeDock::on_deleteLayer_clicked()
{
	if (GuiContext::getInstance().getLayerManager().getCurrentLayer()) {
		QTreeWidgetItem* r=TreeUtil::getTreeWidgetItemFromNode(ui->treeWidget, GuiContext::getInstance().getLayerManager().getCurrentLayer());
		if (r) {
			delete r;
		}
		GuiContext::getInstance().getLayerManager().deleteCurrentLayer();
		setCurrentLayerAsSelected();
		GuiContext::getInstance().updateGlWidget();
	}
}

void SceneTreeDock::on_layerUp_clicked()
{

}

void SceneTreeDock::on_layerDown_clicked()
{

}



void SceneTreeDock::setCurrentLayerAsSelected() {
	NodeLayer* rCurrentNodeLayer=GuiContext::getInstance().getLayerManager().getCurrentLayer();
	if (rCurrentNodeLayer) {
		QTreeWidgetItem *r=TreeUtil::getTreeWidgetItemFromNode(ui->treeWidget, rCurrentNodeLayer);
		if (r && !r->isSelected()) {
			ui->treeWidget->clearSelection();
			r->setSelected(true);
			ui->treeWidget->scrollToItem(r);
		}
	}
	GuiContext::getInstance().updateNodeName(rCurrentNodeLayer);
}

void SceneTreeDock::removeAllLayer() {
	ui->treeWidget->clear();
}

void SceneTreeDock::on_addLayer_clicked()
{
	GuiContext::getInstance().getLayerManager().addNewLayer();
}

void SceneTreeDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		if (rNode && rNode->getNodeType()==NodeType::Layer) {
			NodeLayer* rNodeLayer=static_cast<NodeLayer*>(rNode);
			GuiContext::getInstance().getLayerManager().setCurrentLayer(rNodeLayer);
			GuiContext::getInstance().updateNodeName(rNodeLayer);
			GuiContext::getInstance().updateGlWidget();
		}
	}
}


void SceneTreeDock::setSceneEditable(bool isEditable) {
	ui->addLayer->setEnabled(isEditable);
	ui->deleteLayer->setEnabled(isEditable);
	ui->layerUp->setEnabled(isEditable);
	ui->layerDown->setEnabled(isEditable);
}



QTreeWidgetItem* SceneTreeDock::getSelectedSceneItem() {
	return mTabInfoScene.getSelectedItem();
}

QTreeWidgetItem* SceneTreeDock::getSelectedResourceItem() {
	return mTabInfoResource.getSelectedItem();
}

Node* SceneTreeDock::getSelectedSceneNode() {
	return mTabInfoScene.getSelectedNode();
}
Node* SceneTreeDock::getSelectedResourceNode() {
	return mTabInfoResource.getSelectedNode();
}
Node* SceneTreeDock::getRootSceneNode() {
	return mTabInfoScene.getRootNode();
}
Node* SceneTreeDock::getRootResourceNode() {
	return mTabInfoResource.getRootNode();
}

QTreeWidgetItem* SceneTreeDock::getRootSceneItem() {
	return mTabInfoScene.getRootItem();
}
QTreeWidgetItem* SceneTreeDock::getRootResourceItem() {
	return mTabInfoResource.getRootItem();
}

void SceneTreeDock::updateNodeName(Node* rUpdateNode) {
	if (rUpdateNode->getNodeType()==NodeType::Layer) {
		updateLayerName(static_cast<NodeLayer*>(rUpdateNode));
	} else {
		mTabInfoResource.updateNodeName(rUpdateNode);
	}
}

void SceneTreeDock::updateChildNodes(Node* rNode) {
	mTabInfoScene.updateChildNodes(rNode);
	mTabInfoResource.updateChildNodes(rNode);
}

void SceneTreeDock::addSceneNodeToSelectedItem(Node *rNode) {
	QTreeWidgetItem* r=mTabInfoScene.getSelectedItem();
	if (r==nullptr) {
		r=mTabInfoScene.getRootItem();
	}
	mTabInfoScene.addNode(r,rNode, true);
}

void SceneTreeDock::addSceneNodeToParent(Node *rNode, Node *rParent) {
	QTreeWidgetItem* r=mTabInfoScene.searchTreeWidgetItemByNode(rParent);
	if (r==nullptr) {
		r=mTabInfoScene.getRootItem();
	}
	mTabInfoScene.addNode(r,rNode, false);
}

void SceneTreeDock::eraseSceneNode(Node *rNode) {
    QTreeWidgetItem* r=mTabInfoScene.searchTreeWidgetItemByNode(rNode);
    if (r) {
        mTabInfoScene.deleteItemAndItsChilds(r);
    }
}

void SceneTreeDock::addResourceNodeToSelectedItem(Node *rNode) {
	QTreeWidgetItem* r=mTabInfoResource.getSelectedItem();
	if (r==nullptr) {
		r=mTabInfoResource.getRootItem();
	}
	mTabInfoResource.addNode(r,rNode, true);
}

void SceneTreeDock::addSceneNodeToRootItem(Node *rNode) {
	QTreeWidgetItem	*r=mTabInfoScene.getRootItem();
	mTabInfoScene.addNode(r,rNode, true);
}

void SceneTreeDock::addResourceNodeToRootItem(Node *rNode) {
	QTreeWidgetItem	*r=mTabInfoResource.getRootItem();
	mTabInfoResource.addNode(r,rNode, true);
}

void SceneTreeDock::updateSceneDropdownWithCurrent() {
	int i=0;
	ui->sceneNameOpm->clear();
	vector<string> v=ProjectContext::getInstance().getSceneNames();
	for (auto& s : v) {
		ui->sceneNameOpm->addItem(QString::fromStdString(s));
		if (GuiContext::getInstance().getCurrentScene() && GuiContext::getInstance().getCurrentScene()->getName()==s) {
			ui->sceneNameOpm->setCurrentIndex(i);
		}
		i++;
	}
}

void SceneTreeDock::updateResourceDropdownWithCurrent() {
	int i=0;
	ui->resourceNameOpm->clear();
	vector<string> v=ProjectContext::getInstance().getResourceNames();
	for (auto& s : v) {
		ui->resourceNameOpm->addItem(QString::fromStdString(s));
		if (GuiContext::getInstance().getCurrentResource() && GuiContext::getInstance().getCurrentResource()->getName()==s) {
			ui->resourceNameOpm->setCurrentIndex(i);
		}
		i++;
	}
}

//void SceneTreeDock::switchToScene(Node *rNode) {
//	clearScene();
//	if (rNode) {
//		setSceneEditable(true);
//		mTabInfoScene.addNode(nullptr, rNode, false);
//	}
//	GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(nullptr);
//}
void SceneTreeDock::switchToResource(Node *rNode) {
	clearResource();
	if (rNode) {
		setResourceEditable(true);
		mTabInfoResource.addNode(nullptr, rNode, false);
	}
	GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(nullptr);
}


void SceneTreeDock::setSceneNodeAsSelected(Node *rNode) {
	if (rNode) {
		mTabInfoScene.setItemAsSelectedFromNode(rNode);
		GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	}
}

void SceneTreeDock::on_treeWidgetResources_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	}
}

void SceneTreeDock::on_tabWidget_currentChanged(int index)
{
#if 0
	Node* rNode=nullptr;
	if (index==0) {
		rNode=getSelectedSceneNode();
	} else {
		rNode=getSelectedResourceNode();
	}
	if (rNode) {
		// do not switch if in the current tab nothing is selected/available -> do not remove the current property if anyone is currently outlined
		GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
	}
#endif
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(const QString &arg1)
{
	ignoreparam(arg1);
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(const QString &arg1)
{
	ignoreparam(arg1);
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(int index)
{
	ignoreparam(index);
}

void SceneTreeDock::on_sceneNameOpm_activated(const QString &arg1)
{
	GuiContext::getInstance().setCurrentScene(arg1.toStdString());
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(int index)
{
	ignoreparam(index);
}
void SceneTreeDock::on_resourceNameOpm_activated(const QString &arg1)
{
	GuiContext::getInstance().setCurrentResource(arg1.toStdString());
}

void SceneTreeDock::on_newTexture_clicked()
{
	TextureFrameEditor *rTextureFrameEditor=new TextureFrameEditor(nullptr, this);
	rTextureFrameEditor->show();
}

void SceneTreeDock::on_newAnimation_clicked()
{
	AnimationFrameEditor *rAnimationFrameEditor=new AnimationFrameEditor(nullptr, this);
	rAnimationFrameEditor->show();
}




