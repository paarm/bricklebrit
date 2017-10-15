#include "sceneitemresizemanager.h"
#include "worldcalculator.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneItemResizeManager::SceneItemResizeManager() {
}

void SceneItemResizeManager::startResize(Camera *rCamera, Node2d* rNode2d, int rStartMouseX, int rStartMouseY) {
	mIsOnResize=false;
	if (rCamera && rNode2d && rNode2d->getParent() && (rNode2d->getParent()->getNodeType()==NodeType::Sprite ||rNode2d->getParent()->getNodeType()==NodeType::Scene)) {
		mIsOnResize=true;
		mCamera=rCamera;
		mStartMouseX=rStartMouseX;
		mStartMouseY=rStartMouseY;
		GLMVector3 worldPosStart=mCamera->unproject(mStartMouseX, mStartMouseY);
		//PointFloat rCurrentWorldLocationBox[4]=rNode2d->getCurrentWorldLocationBox();
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
	mNode2d=nullptr;
	mStartScale.x=0.0;
	mStartScale.y=0.0;
	mStartSize.x=0.0;
	mStartSize.y=0.0;
}

bool SceneItemResizeManager::updateResize(int rMouseX, int rMouseY) {
	bool rv=false;
	if (mIsOnResize) {
		GLMVector3 worldPosCurrent=mCamera->unproject(rMouseX, rMouseY);
		float distanceX=worldPosCurrent.getX()-mStartWorldX;
		float distanceY=worldPosCurrent.getY()-mStartWorldY;
		//cout <<"World distance X: "<<distanceX <<" Y:" <<distanceY <<endl;
		PointInt distanceLocal=WorldCalculator::getLocalPosFromWorldPos(mNode2d_parent, PointFloat(distanceX, distanceY), false);
		//cout <<distanceLocal.x << "y: " << distanceLocal.y <<endl;


		//PointInt pp=WorldCalculator::getLocalPosFromWorldPos(mNode2d_parent, PointFloat(worldPosCurrent.getX(), worldPosCurrent.getY()), false);
		mNode2d->setSize(PointInt(mStartSize.x+distanceLocal.x*2.0, mStartSize.y+distanceLocal.y*2.0));
		rv=true;
	}
	return rv;
}

bool SceneItemResizeManager::isOnResize() {
	return mIsOnResize;
}
