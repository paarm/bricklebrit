#include "selectionmanager.h"
#include <algorithm>
#include "../guicontext.h"

SelectionManager::SelectionManager()
{
}

void SelectionManager::deselectAllNodes() {
	GuiContext::getInstance().getMainWindow().getSelectionDock().removeAllNodes();
	for(Node* rNode : mSelectedSceneNodes) {
		Node2d *rNode2d=static_cast<Node2d*>(rNode);
		rNode2d->updateSelection(false, {0,0,0});
	}
	mSelectedSceneNodes.clear();
	mLatestSelected=nullptr;
}

void SelectionManager::deselectAllIfChildOf(Node *rParent) {
	if (rParent) {
		vector<Node*> toDeselect;
		for(auto rNode : mSelectedSceneNodes) {
			if (rNode->isThisNodeOrParentOrGrandParentOf(rParent)) {
				toDeselect.push_back(rNode);
			}
		}
		for (Node*rNode : toDeselect) {
			deselectNode(rNode);
		}
	}
}

void SelectionManager::deselectNode(Node *rDeselectedNode) {
	if (rDeselectedNode!=nullptr) {
		const auto &it=std::find_if(mSelectedSceneNodes.begin(), mSelectedSceneNodes.end(), [rDeselectedNode] (Node* entry) {
			return rDeselectedNode==entry;
		}
		);
		if (it!=mSelectedSceneNodes.end()) {
			Node2d *rNode2d=static_cast<Node2d*>(rDeselectedNode);
			rNode2d->updateSelection(false,{0,0,0});
			mSelectedSceneNodes.erase(it);
			mSelectedSceneNodes.shrink_to_fit();
		}
		if (mLatestSelected!=nullptr && mLatestSelected==rDeselectedNode) {
			setAsLatestSelected(nullptr);
		}
		GuiContext::getInstance().getMainWindow().getSelectionDock().removeNode(rDeselectedNode);
	}
}

bool SelectionManager::isNodeSelected(Node* rTestNode) {
	bool rv=false;
	if (rTestNode!=nullptr) {
		for(auto rNode : mSelectedSceneNodes) {
			if (rNode==rTestNode) {
				rv=true;
				break;
			}
		}
	}
	return rv;
}

void SelectionManager::replaceAllSelectedWithNode(Node *rSelectedNode) {
	deselectAllNodes();
	setNodeAsSelected(rSelectedNode);
}

void SelectionManager::setNodeAsSelected(Node* rSelectedNode) {
	if (!isNodeSelected(rSelectedNode)) {
		if (rSelectedNode!=nullptr) {
			Node2d *rNode2d=static_cast<Node2d*>(rSelectedNode);

			mSelectedSceneNodes.push_back(rSelectedNode);
			//rNode2d->updateSelection(true, {0,0,100});
			setAsLatestSelected(nullptr);
			GuiContext::getInstance().getMainWindow().getSelectionDock().addNode(rSelectedNode);
		}
	}
}

void SelectionManager::startBulkSelection() {
	deselectAllNodes();
	setAsLatestSelected(nullptr);
}

void SelectionManager::setNodeAsSelectedInBulk(Node *rSelectedNode) {
	if (!isNodeSelected(rSelectedNode)) {
		if (rSelectedNode!=nullptr) {
			Node2d *rNode2d=static_cast<Node2d*>(rSelectedNode);

			rNode2d->updateSelection(true, {0,1.0/255.0*107.0,1.0/255.0*180.0});

			mSelectedSceneNodes.push_back(rSelectedNode);
			//mSelectedBulkNodes.push_back(rSelectedNode);
		}
	}
}

void SelectionManager::finishBulkSelection() {
	setAsLatestSelected(nullptr);
	GuiContext::getInstance().getMainWindow().getSelectionDock().addNodes(mSelectedSceneNodes);
	//mSelectedBulkNodes.clear();
}

//vector<Node*> &SelectionManager::getCurrentBulkSelectionNodes() {
//	return mSelectedBulkNodes;
//}
void SelectionManager::setAsLatestSelected(Node* rNode) {
	Node* rLatestSelectedLast=mLatestSelected;
	const auto &it=std::find_if(mSelectedSceneNodes.begin(), mSelectedSceneNodes.end(), [rNode] (Node* entry) {
		return rNode==entry;
	});
	if (rNode && it!=mSelectedSceneNodes.end()) {
		mLatestSelected=*it;
	} else if (!mSelectedSceneNodes.empty()) {
		mLatestSelected=mSelectedSceneNodes.back();
	} else {
		mLatestSelected=nullptr;
	}

	if (mLatestSelected && mLatestSelected!=rLatestSelectedLast) {
		for(auto rNode : mSelectedSceneNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);

			if (rNode2d==mLatestSelected) {
				rNode2d->updateSelection(true, {0,1.0/255.0*200.0,1.0/255.0*58.0});
			} else {
				rNode2d->updateSelection(true, {0,1.0/255.0*107.0,1.0/255.0*180.0});
			}
		}
	}

	GuiContext::getInstance().updateNodeName(mLatestSelected);
}

Node* SelectionManager::getLatestSelected() {
	return mLatestSelected;
}

bool SelectionManager::haveSelectedNodes() {
	bool rv=false;
	if (mSelectedSceneNodes.size()>0) {
		rv=true;
	}
	return rv;
}

vector<Node*> &SelectionManager::getSelectedNodes() {
	return mSelectedSceneNodes;
}

int SelectionManager::moveToCurrentLayer() {
    int movedNodes=0;
    NodeLayer* rNodeLayer=GuiContext::getInstance().getLayerManager().getCurrentLayer();
    if (rNodeLayer) {
        for(auto rNode : mSelectedSceneNodes) {
            Node2d *rNode2d=static_cast<Node2d*>(rNode);
            if (rNode2d->getParent()!=rNodeLayer) {
                rNodeLayer->moveChildNode(rNode2d);
                glm::vec4 worldPos=rNode2d->getLocationInfo().rWorldLocationCenter;
                glm::vec4 localPos=rNodeLayer->getLocalPosFromWorldPos(worldPos.x, worldPos.y, false);
                rNode2d->setPosition(PointInt(localPos.x, localPos.y));
                movedNodes++;
            }
        }
    }
    return movedNodes;
}
