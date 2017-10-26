#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <bricklelib.h>

class SelectionManager
{
private:
	vector<Node*>			mSelectedSceneNodes;
	Node*					mLatestSelected=nullptr;
public:
	SelectionManager();
	void deselectAllNodes();
	void deselectAllIfChildOf(Node *rParent);
	void deselectNode(Node *rDeselectedNode);
	bool isNodeSelected(Node* rTestNode);
	void replaceAllSelectedWithNode(Node *rSelectedNode);
	void setNodeAsSelected(Node* rSelectedNode);
	bool haveSelectedNodes();
	const vector<Node*> getSelectedNodes();
	Node* getLatestSelected();
	void setAsLatestSelected(Node* rNode);

};

#endif // SELECTIONMANAGER_H
