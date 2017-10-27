#ifndef WORLDCALCULATOR_H
#define WORLDCALCULATOR_H
#include <bricklelib.h>

class WorldCalculator
{
public:
	static void updateNodeMatrix(GLMMatrix4 parentMatrix, Node* rNode);
	static bool isBoxIntersecting(PointFloat *rCurrentWorldLocationBox, float worldX, float worldY, float worldDX, float worldDY);
	static void intersectTestForScene(vector<Node*> &rv, NodeScene* rNodeScene, float worldX, float worldY, float worldDX, float worldDY, bool selectOnlyFirst);
	static int calcGridPos(int worldPos, int gridSize, int gridOffset);
	static PointInt getLocalPosFromWorldPos(Node2d *rNode2dParent, PointFloat rWorldPos, bool rUseSnapToGridSetting);
	static void buildFlatNodeList(vector<Node*> &rv, Node *rParentNode);
};

#endif // WORLDCALCULATOR_H
