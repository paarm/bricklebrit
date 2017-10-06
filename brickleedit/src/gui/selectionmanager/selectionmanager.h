#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <bricklelib.h>

class SelectionManager
{
private:
	vector<Node*>	mSelectedSceneNodes;
	bool			mMouseOverSelection=false;
public:
	SelectionManager();
	void deselectAllNodes();
	void deselectNode(Node *rDeselectedNode);
	bool isNodeSelected(Node* rTestNode);
	void replaceAllSelectedWithNode(Node *rSelectedNode);
	void setNodeAsSelected(Node* rSelectedNode);
	bool haveSelectedNodes();
	const vector<Node*> getSelectedNodes();
	void setIsMouseOverSelection(bool isMouseOverSelection);
	bool isMouseOverSelection();
};

#endif // SELECTIONMANAGER_H
