#ifndef SCENEITEMRESIZEMANAGER_H
#define SCENEITEMRESIZEMANAGER_H
#include <bricklelib.h>


class SceneItemResizeManager
{
private:
	Camera  *mCamera=nullptr;
	bool	mIsOnResize=false;
	Node2d	*mNode2d=nullptr;
	Node2d	*mNode2d_parent=nullptr;
	int		mStartMouseX=0;
	int		mStartMouseY=0;
	float	mStartWorldX;
	float	mStartWorldY;
	PointFloat	mStartScale;
	PointInt	mStartSize;
public:
	SceneItemResizeManager();
	void startResize(Camera *rCamera, Node2d* rNode2d, int rStartMouseX, int rStartMouseY);
	bool updateResize(int rMouseX, int rMouseY);
	void stopResize();
	bool isOnResize();
};

#endif // SCENEITEMRESIZEMANAGER_H
