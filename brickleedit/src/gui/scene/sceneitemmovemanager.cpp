#include "sceneitemmovemanager.h"
#include "worldcalculator.h"
#include "../guicontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneItemMoveManager::SceneItemMoveManager() {
}

void SceneItemMoveManager::startMove(Camera *rCamera, int rStartMouseX, int rStartMouseY) {
	mIsOnMove=true;
	mCamera=rCamera;
	mStartMouseX=rStartMouseX;
	mStartMouseY=rStartMouseY;
	GLMVector3 worldPosStart=mCamera->unproject(mStartMouseX, mStartMouseY);
	mStartWorldX=worldPosStart.getX();
	mStartWorldY=worldPosStart.getY();

	mStartPositionsOnMove.clear();
	const vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
	for (Node* n : v) {
		if (n->getNodeType()==NodeType::Sprite) {
			NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);
			mStartPositionsOnMove[rNodeSprite]=rNodeSprite->getCurrentCenter();
		}
	}
}

void SceneItemMoveManager::stopMove() {
	mIsOnMove=false;
	mStartMouseX=0;
	mStartMouseY=0;
	mStartWorldX=0;
	mStartWorldY=0;
	mStartPositionsOnMove.clear();
}

bool SceneItemMoveManager::updateMove(int rMouseX, int rMouseY) {
	bool rv=false;
	if (mIsOnMove) {
		GLMVector3 worldPosCurrent=mCamera->unproject(rMouseX, rMouseY);
		float distanceX=mStartWorldX-worldPosCurrent.getX();
		float distanceY=mStartWorldY-worldPosCurrent.getY();

		vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
		for (Node *n : v) {
			if (n->getNodeType()==NodeType::Sprite) {
				NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);
				if (rNodeSprite->getParent() && (
							rNodeSprite->getParent()->getNodeType()==NodeType::Sprite || rNodeSprite->getParent()->getNodeType()==NodeType::Scene )) {
					Node2d* rNode2dParent=static_cast<Node2d*>(rNodeSprite->getParent());
					PointFloat pos=getStartWorldPositionOfNode(rNodeSprite);
					pos.x-=distanceX;
					pos.y-=distanceY;
					PointInt pp=WorldCalculator::getLocalPosFromWorldPos(rNode2dParent, pos, true);
					rNodeSprite->setPosition(pp);
				}
			}
		}
		rv=true;
	}

	return rv;
}


PointFloat SceneItemMoveManager::getStartWorldPositionOfNode(Node* rNode) {
	if (mStartPositionsOnMove.find(rNode)!=mStartPositionsOnMove.end()) {
		return mStartPositionsOnMove[rNode];
	}
	return PointFloat();
}
