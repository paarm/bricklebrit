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

void SceneTreeDock::clearResource() {
	mTabInfoResource.clear();
	setResourceEditable(false);
}

void SceneTreeDock::setSceneEditable(bool isEditable) {
	ui->newSprite->setEnabled(isEditable);
}

void SceneTreeDock::setResourceEditable(bool isEditable) {
	ui->newTexture->setEnabled(isEditable);
	ui->newAnimation->setEnabled(isEditable);
}

void SceneTreeDock::setProjectAvailable(bool isActive) {
	ui->sceneMenu->setEnabled(isActive);
	ui->resourceMenu->setEnabled(isActive);
}


int SceneTreeDock::getActiveTreeTabIndex() {
	return ui->tabWidget->currentIndex();
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
	mTabInfoScene.updateNodeName(rUpdateNode);
	mTabInfoResource.updateNodeName(rUpdateNode);
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

void SceneTreeDock::switchToScene(Node *rNode) {
	clearScene();
	if (rNode) {
		setSceneEditable(true);
		mTabInfoScene.addNode(nullptr, rNode, false);
	}
	GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(nullptr);
}
void SceneTreeDock::switchToResource(Node *rNode) {
	clearResource();
	if (rNode) {
		setResourceEditable(true);
		mTabInfoResource.addNode(nullptr, rNode, false);
	}
	GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(nullptr);
}

void SceneTreeDock::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column>=0) {
		Node* rNode=TreeUtil::getNodeFromTreeItem(item);
		GuiContext::getInstance().getMainWindow().getPropertyTreeDock().setPropertiesForNode(rNode);
		GuiContext::getInstance().replaceSelectedSceneNode(rNode);
		GuiContext::getInstance().updateGlWidget();
	}
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
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(const QString &arg1)
{
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(const QString &arg1)
{
}

void SceneTreeDock::on_sceneNameOpm_currentIndexChanged(int index)
{
	string name="";
	if (index>=0) {
		name=ui->sceneNameOpm->itemText(index).toStdString();
	}
	GuiContext::getInstance().setCurrentScene(name);
}

void SceneTreeDock::on_resourceNameOpm_currentIndexChanged(int index)
{
	string name="";
	if (index>=0) {
		name=ui->resourceNameOpm->itemText(index).toStdString();
	}
	GuiContext::getInstance().setCurrentResource(name);
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

void SceneTreeDock::on_newSprite_clicked()
{
	SpriteEditor *rSpriteEditor=new SpriteEditor(nullptr, this);
	rSpriteEditor->show();
}
