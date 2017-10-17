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
    float	mStartWorldX=0;
    float	mStartWorldY=0;
    float   mStartWorldOffsetX=0;
    float   mStartWorldOffsetY=0;
	PointFloat	mStartScale;
	PointInt	mStartSize;
	bool	mXonly=false;
	bool	mYonly=false;
public:
	SceneItemResizeManager();
	void startResize(PointFloat* rResizeHandle, bool rXonly, bool rYonly, Camera *rCamera, Node2d* rNode2d, int rStartMouseX, int rStartMouseY);
	bool updateResize(bool rSyncXY, int rMouseX, int rMouseY);
	void stopResize();
	bool isOnResize();
	Node2d* getNodeForResize();
};

#endif // SCENEITEMRESIZEMANAGER_H
