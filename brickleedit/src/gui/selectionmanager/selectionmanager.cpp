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
		rNode2d->updateSelection(false);
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
			rNode2d->updateSelection(false);
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
			rNode2d->updateSelection(true);

			mSelectedSceneNodes.push_back(rSelectedNode);
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
			rNode2d->updateSelection(true);

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


