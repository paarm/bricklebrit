#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <bricklelib.h>

class SelectionManager
{
private:
	vector<Node*>			mSelectedSceneNodes;
public:
	SelectionManager();
	void deselectAllNodes();
	void deselectNode(Node *rDeselectedNode);
	bool isNodeSelected(Node* rTestNode);
	void replaceAllSelectedWithNode(Node *rSelectedNode);
	void setNodeAsSelected(Node* rSelectedNode);
	bool haveSelectedNodes();
	const vector<Node*> getSelectedNodes();
};

#endif // SELECTIONMANAGER_H
