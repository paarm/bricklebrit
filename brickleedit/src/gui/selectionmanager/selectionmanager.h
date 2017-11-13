#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <bricklelib.h>

class SelectionManager
{
private:
	vector<Node*>			mSelectedSceneNodes;
	//vector<Node*>			mSelectedBulkNodes;
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
	vector<Node*> &getSelectedNodes();
	//vector<Node*> &getCurrentBulkSelectionNodes();
	Node* getLatestSelected();
	void setAsLatestSelected(Node* rNode);
	void startBulkSelection();
	void setNodeAsSelectedInBulk(Node *rSelectedNode);
	void finishBulkSelection();


};

#endif // SELECTIONMANAGER_H
