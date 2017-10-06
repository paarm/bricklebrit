#include "selectionmanager.h"
#include <algorithm>

SelectionManager::SelectionManager()
{
}

void SelectionManager::deselectAllNodes() {
	mSelectedSceneNodes.clear();
}

void SelectionManager::deselectNode(Node *rDeselectedNode) {
	if (rDeselectedNode!=nullptr) {
		const auto &it=std::find_if(mSelectedSceneNodes.begin(), mSelectedSceneNodes.end(), [rDeselectedNode] (Node* entry) {
			return rDeselectedNode==entry;
		}
		);
		if (it!=mSelectedSceneNodes.end()) {
			mSelectedSceneNodes.erase(it);
		}
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
			mSelectedSceneNodes.push_back(rSelectedNode);
		}
	}
}

bool SelectionManager::haveSelectedNodes() {
	bool rv=false;
	if (mSelectedSceneNodes.size()>0) {
		rv=true;
	}
	return rv;
}

const vector<Node*> SelectionManager::getSelectedNodes() {
	return mSelectedSceneNodes;
}

void SelectionManager::setIsMouseOverSelection(bool isMouseOverSelection) {
	mMouseOverSelection=isMouseOverSelection;
}

bool SelectionManager::isMouseOverSelection() {
	return mMouseOverSelection;
}

