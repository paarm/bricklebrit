#ifndef SCENEBULKSELECTMANAGER_H
#define SCENEBULKSELECTMANAGER_H
#include <bricklelib.h>

class SceneBulkSelectManager
{
private:
	Camera  *mCamera=nullptr;
	bool	mIsOnSelect=false;
	int		mStartMouseX=0;
	int		mStartMouseY=0;
	float	mStartWorldX;
	float	mStartWorldY;
	int		mCurrentMouseX=0;
	int		mCurrentMouseY=0;
	float	mCurrentWorldX;
	float	mCurrentWorldY;
public:
	SceneBulkSelectManager();
	void startSelect(Camera *rCamera, int rStartMouseX, int rStartMouseY);
	bool updateSelect(int rMouseX, int rMouseY);
	void stopSelect();
	bool isOnSelect();
	float getStartWorldX();
	float getStartWorldY();
	float getCurrentWorldX();
	float getCurrentWorldY();
};

#endif // SCENEBULKSELECTMANAGER_H
