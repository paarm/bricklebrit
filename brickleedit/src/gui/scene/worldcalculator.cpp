#include "worldcalculator.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void WorldCalculator::updateNodeMatrix(GLMMatrix4 parentMatrix, Node* rNode) {
	glm::mat4 pM=glm::make_mat4x4(parentMatrix.getPointer());
	glm::mat4 m(1.0);
	GLMMatrix4 pMM;
	if (rNode->getNodeType()==NodeType::Sprite) {
		NodeSprite *rNodeSprite=static_cast<NodeSprite*>(rNode);
		float angle=rNodeSprite->getRotation();
		float scaleX=rNodeSprite->getScale().x;
		float scaleY=rNodeSprite->getScale().y;
		if (angle>360.0) {
			angle=angle/360.0;
		} else if (angle<0.0) {
			angle=360.0+angle;
		}
		m=glm::translate(m, glm::vec3(rNodeSprite->getPosition().x,rNodeSprite->getPosition().y,0.0));
		m=glm::rotate(m, glm::radians(angle), glm::vec3(0.0,0.0,1.0));
		//if (scaleX!=1.0 || scaleY!=1.0) {
		m=glm::scale(m, glm::vec3(scaleX, scaleY, 1.0));
		//}
		pM*=m;
		pMM.setFromPointer(glm::value_ptr(pM));
		rNodeSprite->setCurrentModelMatrix(pMM);
	} else if (rNode->getNodeType()==NodeType::Scene) {
		NodeScene * rNodeScene=static_cast<NodeScene*>(rNode);
		rNodeScene->setCurrentModelMatrix(pMM);
	}
	int childCount=rNode->getChildCount();
	for (int i=0;i<childCount;i++) {
		Node *rNodeChild=rNode->getNodeFromIndex(i);		
		updateNodeMatrix(pMM, rNodeChild);
	}
}

bool WorldCalculator::isNodeIntersecting(Node* rNode, float worldX, float worldY) {
	bool rv=false;
	if (rNode) {
		NodeSprite *rNodeSprite=static_cast<NodeSprite*>(rNode);
		vector<PointFloat> v1;
		vector<PointFloat> v2;
		v2.emplace_back(worldX, worldY);
		v2.emplace_back(worldX+1.0, worldY+1.0);
		v1.emplace_back(rNodeSprite->getCurrentLeftTop());
		v1.emplace_back(rNodeSprite->getCurrentRightTop());
		v1.emplace_back(rNodeSprite->getCurrentRightBottom());
		v1.emplace_back(rNodeSprite->getCurrentLeftBottom());
		v1.emplace_back(rNodeSprite->getCurrentLeftTop());
		if (Intersection::isPolygonsIntersecting(v1, v2)) {
			rv=true;
		}
	}
	return rv;
}


void WorldCalculator::intersectTestForNode(vector<Node*> &rv, Node* rNode, float worldX, float worldY) {
	if (rNode) {
		if (rNode->getNodeType()==NodeType::Sprite) {
			if (isNodeIntersecting(rNode, worldX, worldY)) {
				rv.push_back(rNode);
			}
		}
		int childCount=rNode->getChildCount();
		for (int i=0;i<childCount;i++) {
			Node *rNodeChild=rNode->getNodeFromIndex(i);
			WorldCalculator::intersectTestForNode(rv, rNodeChild, worldX, worldY);
		}
	}
}

int WorldCalculator::calcGridPos(int worldPos, int gridSize, int gridOffset) {
	int rv=worldPos;
	if (gridSize>0) {
		int gridField=0;
		if (worldPos>0) {
			gridField=(worldPos+gridSize/2)/gridSize;
		} else if (worldPos<0) {
			gridField=(worldPos-gridSize/2)/gridSize;
		}
		rv=gridField*gridSize;//+rGridState.gridX/2;
		if (gridOffset>0) {
			if (worldPos>=0) {
				rv+=gridOffset;
			} else {
				rv-=gridOffset;
			}
		}
	}
	return rv;
}
