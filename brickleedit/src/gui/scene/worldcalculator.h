#ifndef WORLDCALCULATOR_H
#define WORLDCALCULATOR_H
#include <bricklelib.h>

class WorldCalculator
{
public:
	static void updateNodeMatrix(GLMMatrix4 parentMatrix, Node* rNode);
	static bool isBoxIntersecting(PointFloat *rCurrentWorldLocationBox, float worldX, float worldY);
	static void intersectTestForNode(vector<Node*> &rv, Node* rNode, float worldX, float worldY);
	static int calcGridPos(int worldPos, int gridSize, int gridOffset);
	static PointInt getLocalPosFromWorldPos(Node2d *rNode2dParent, PointFloat rWorldPos, bool rUseSnapToGridSetting);

};

#endif // WORLDCALCULATOR_H
