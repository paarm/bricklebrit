#include "sceneitemresizemanager.h"
#include "worldcalculator.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneItemResizeManager::SceneItemResizeManager() {
}

void SceneItemResizeManager::startResize(HandleLocationInfo* rHandleLocationInfo, bool rXonly, bool rYonly, Camera *rCamera, Node2d* rNode2d, int rStartMouseX, int rStartMouseY) {
	mIsOnResize=false;
	if (rCamera && rNode2d && rNode2d->getParent() && (rNode2d->getParent()->getNodeType()==NodeType::Layer || rNode2d->getParent()->getNodeType()==NodeType::Sprite || rNode2d->getParent()->getNodeType()==NodeType::Scene)) {
		mIsOnResize=true;
		mCamera=rCamera;
		mStartMouseX=rStartMouseX;
		mStartMouseY=rStartMouseY;
		GLMVector3 worldPosStart=mCamera->unproject(mStartMouseX, mStartMouseY);
		mStartWorldOffsetX=worldPosStart.getX()-rHandleLocationInfo->rWorldLocationBox[0].x;
		mStartWorldOffsetY=worldPosStart.getY()-rHandleLocationInfo->rWorldLocationBox[0].y;
		mXonly=rXonly;
		mYonly=rYonly;
        mStartWorldX=worldPosStart.getX();
		mStartWorldY=worldPosStart.getY();
		mNode2d=rNode2d;
		mNode2d_parent=static_cast<Node2d*>(rNode2d->getParent());
		mStartScale=rNode2d->getScale();
		mStartSize=rNode2d->getSize();
	}
}

void SceneItemResizeManager::stopResize() {
	mIsOnResize=false;
	mStartMouseX=0;
	mStartMouseY=0;
	mStartWorldX=0;
	mStartWorldY=0;
    mStartWorldOffsetX=0;
    mStartWorldOffsetY=0;
	mNode2d=nullptr;
	mNode2d_parent=nullptr;
	mStartScale.x=0.0;
	mStartScale.y=0.0;
	mStartSize.x=0.0;
	mStartSize.y=0.0;
	mXonly=false;
	mYonly=false;

}

bool SceneItemResizeManager::updateResize(bool rSyncXY, int rMouseX, int rMouseY) {
	bool rv=false;
	if (mIsOnResize) {
		GLMVector3 worldPosCurrent=mCamera->unproject(rMouseX, rMouseY);
		PointFloat wp();
		glm::vec4 pv=mNode2d->getLocalPosFromWorldPos(worldPosCurrent.getX()-mStartWorldOffsetX, worldPosCurrent.getY()-mStartWorldOffsetY, false);
		int diffX=mStartSize.x-pv.x*2;
		int diffY=mStartSize.y-pv.y*2;
		if (mXonly) {
			diffY=0;
		}
		if (mYonly) {
			diffX=0;
		}
		if (!mXonly && !mYonly && rSyncXY) {
			// keep proportion
			if (diffY!=0) {
				float ppY=1.0;
				ppY=(float(mStartSize.y)-float(diffY))/float(mStartSize.y);
				mNode2d->setSize(PointInt(mStartSize.x*ppY, mStartSize.y-diffY));
			}
		} else {
			mNode2d->setSize(PointInt(mStartSize.x-diffX, mStartSize.y-diffY));
		}
		rv=true;
	}
	return rv;
}

bool SceneItemResizeManager::isOnResize() {
	return mIsOnResize;
}

Node2d* SceneItemResizeManager::getNodeForResize() {
	return mNode2d;
}
