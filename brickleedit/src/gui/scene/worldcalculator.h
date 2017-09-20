#ifndef WORLDCALCULATOR_H
#define WORLDCALCULATOR_H
#include <bricklelib.h>

class WorldCalculator
{
public:
	static void updateNodeMatrix(GLMMatrix4 parentMatrix, Node* rNode);
	static void intersectTestForNode(vector<Node*> &rv, Node* rNode, float worldX, float worldY);
};

#endif // WORLDCALCULATOR_H
