#include "scenebulkselectmanager.h"
#include "worldcalculator.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneBulkSelectManager::SceneBulkSelectManager()
{

}

void SceneBulkSelectManager::startSelect(Camera *rCamera, int rStartMouseX, int rStartMouseY) {
	mIsOnSelect=true;
	mCamera=rCamera;
	mCurrentMouseX=mStartMouseX=rStartMouseX;
	mCurrentMouseY=mStartMouseY=rStartMouseY;
	GLMVector3 worldPosStart=mCamera->unproject(mStartMouseX, mStartMouseY);
	mCurrentWorldX=mStartWorldX=worldPosStart.getX();
	mCurrentWorldY=mStartWorldY=worldPosStart.getY();
}

void SceneBulkSelectManager::stopSelect() {
	mIsOnSelect=false;
	mStartMouseX=0;
	mStartMouseY=0;
	mStartWorldX=0;
	mStartWorldY=0;
	mCurrentMouseX=0;
	mCurrentMouseY=0;
	mCurrentWorldX=0;
	mCurrentWorldY=0;
}

float SceneBulkSelectManager::getStartWorldX() {
	return mStartWorldX;
}
float SceneBulkSelectManager::getStartWorldY() {
	return mStartWorldY;
}
float SceneBulkSelectManager::getCurrentWorldX() {
	return mCurrentWorldX;
}
float SceneBulkSelectManager::getCurrentWorldY() {
	return mCurrentWorldY;
}


bool SceneBulkSelectManager::updateSelect(int rMouseX, int rMouseY) {
	bool rv=false;
	if (mIsOnSelect) {
		GLMVector3 worldPosCurrent=mCamera->unproject(rMouseX, rMouseY);
		mCurrentMouseX=rMouseX;
		mCurrentMouseY=rMouseY;
		mCurrentWorldX=worldPosCurrent.getX();
		mCurrentWorldY=worldPosCurrent.getY();
#if 0
		float distanceX=mStartWorldX-worldPosCurrent.getX();
		float distanceY=mStartWorldY-worldPosCurrent.getY();

		vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
		for (Node *n : v) {
			if (n->getNodeType()==NodeType::Sprite) {
				NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);
				if (rNodeSprite->getParent() && (
							rNodeSprite->getParent()->getNodeType()==NodeType::Layer || rNodeSprite->getParent()->getNodeType()==NodeType::Sprite || rNodeSprite->getParent()->getNodeType()==NodeType::Scene )) {
					Node2d* rNode2dParent=static_cast<Node2d*>(rNodeSprite->getParent());
					PointFloat pos=getStartWorldPositionOfNode(rNodeSprite);
					pos.x-=distanceX;
					pos.y-=distanceY;
					PointInt pp=WorldCalculator::getLocalPosFromWorldPos(rNode2dParent, pos, true);
					rNodeSprite->setPosition(pp);
				}
			}
		}
#endif
		rv=true;
	}
	return rv;
}

bool SceneBulkSelectManager::isOnSelect() {
	return mIsOnSelect;
}

