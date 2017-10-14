#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <bricklelib.h>

class SelectionManager
{
private:
	vector<Node*>			mSelectedSceneNodes;
	map<Node*, PointFloat> mStartPositionsOnMove;
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
	void setStartWorldPositionOfNode(Node* rNode, PointFloat &rCurrentWorldPosition);
	PointFloat getStartWorldPositionOfNode(Node* rNode);


	void setIsMouseOverSelection(bool isMouseOverSelection);
	bool isMouseOverSelection();

};

#endif // SELECTIONMANAGER_H
