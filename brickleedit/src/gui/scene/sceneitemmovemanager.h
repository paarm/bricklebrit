#ifndef SCENEITEMMOVEMANAGER_H
#define SCENEITEMMOVEMANAGER_H
#include <bricklelib.h>

class SceneItemMoveManager
{
private:
	Camera  *mCamera=nullptr;
	bool	mIsOnMove=false;
	int		mStartMouseX=0;
	int		mStartMouseY=0;
	float	mStartWorldX;
	float	mStartWorldY;
	map<Node*, PointFloat> mStartPositionsOnMove;
	PointFloat getStartWorldPositionOfNode(Node* rNode);
public:
	SceneItemMoveManager();
	void startMove(Camera *rCamera, int rStartMouseX, int rStartMouseY);
	bool updateMove(int rMouseX, int rMouseY);
	void stopMove();
	bool isOnMove();
};

#endif // SCENEITEMMOVEMANAGER_H
