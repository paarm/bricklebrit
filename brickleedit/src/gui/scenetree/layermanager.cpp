#include "layermanager.h"
#include "../guicontext.h"
#include <string>
LayerManager::LayerManager()
{

}
void LayerManager::currentSceneChanged() {
	GuiContext::getInstance().getMainWindow().getSceneTreeDock().removeAllLayer();
	mCurrentLayer=nullptr;
	NodeScene* rNodeScene=GuiContext::getInstance().getCurrentScene();
	if (rNodeScene) {
		GuiContext::getInstance().getMainWindow().getSceneTreeDock().setSceneEditable(true);
		vector<Node*> v=rNodeScene->getChildNodesWithNodeType(NodeType::Layer);
		for (Node *n: v) {
			NodeLayer *rNodeLayer=static_cast<NodeLayer*>(n);
			if (mCurrentLayer==nullptr) {
				setCurrentLayer(rNodeLayer);
			}
			GuiContext::getInstance().getMainWindow().getSceneTreeDock().addLayerNode(rNodeLayer);
		}
	} else {
		GuiContext::getInstance().getMainWindow().getSceneTreeDock().setSceneEditable(false);
	}
}
void LayerManager::addNewLayer() {
	NodeScene* rNodeScene=GuiContext::getInstance().getCurrentScene();
	if (rNodeScene) {
		vector<Node*> v=rNodeScene->getChildNodesWithNodeType(NodeType::Layer);
		size_t cnt=v.size();
		NodeLayer* rNodeLayer=static_cast<NodeLayer*>(getInstanceFromNodeType(NodeType::Layer, true));
		rNodeLayer->setName("Layer "+std::to_string(cnt+1));
		rNodeScene->addChildNode(rNodeLayer);
		setCurrentLayer(rNodeLayer);
		GuiContext::getInstance().getMainWindow().getSceneTreeDock().addLayerNode(rNodeLayer);
	}
}

void LayerManager::setCurrentLayer(NodeLayer* rNodeLayer) {
	mCurrentLayer=rNodeLayer;
	GuiContext::getInstance().setCurrentPaintCanvas(mCurrentLayer, false);
}

NodeLayer* LayerManager::getCurrentLayer() {
	return mCurrentLayer;
}

void LayerManager::setLayerVisible(NodeLayer *rNodeLayer, bool rIsVisible) {
	rNodeLayer->setVisible(rIsVisible);
}

void LayerManager::deleteCurrentLayer() {
	if (mCurrentLayer) {
		GuiContext::getInstance().getSelectionManager().deselectAllNodes();
		GuiContext::getInstance().eraseSceneNodeWithId(mCurrentLayer->getId());
		bool haveLayer=false;

		NodeScene* rNodeScene=GuiContext::getInstance().getCurrentScene();
		if (rNodeScene) {
			vector<Node*> v=rNodeScene->getChildNodesWithNodeType(NodeType::Layer);
			if (!v.empty()) {
				NodeLayer* rNodeLayer=static_cast<NodeLayer*>(v.back());
				setCurrentLayer(rNodeLayer);
				haveLayer=true;
			}
		}
		if (!haveLayer) {
			setCurrentLayer(nullptr);
		}
	}
}

