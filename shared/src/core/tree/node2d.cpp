#include "node2d.h"
#include "../texture/textureutil.h"
#include "../math/intersection.h"
Node2d::~Node2d() {
	cleanup();
}

void Node2d::initialSetupAfterLoadRecursive() {
	updatePosition(getPosition().x, getPosition().y, 0.0);
	updateSize(getSize().x, getSize().y);
	updateScale(getScale().x, getScale().y);
	updateRotation(getRotation());
	updateVisible(getVisible());
	updateFrameRef(getFrameRef().resourcefile, getFrameRef().textureid, getFrameRef().frame);
	updateFlipX(getFlipX());
	updateFlipY(getFlipY());
	if (getChildCount()>0) {
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			rNode2d->initialSetupAfterLoadRecursive();
		}
	}
}

//Position
void Node2d::setPosition(const PointInt& r) {
	setPropertyPointInt ("Position", r);
	updatePosition(float(r.x), float(r.y), 0.0);
}
void Node2d::setPosition() {
	setPropertyPointInt ("Position");
	updatePosition(0.0, 0.0, 0.0);
}
PointInt& Node2d::getPosition() {
	return getPropertyPointInt ("Position")->value;
}

//Size
void Node2d::setSize(const PointInt& r) {
	setPropertyPointInt ("Size", r);
	updateSize(float(r.x), float(r.y));
}
void Node2d::setSize() {
	setPropertyPointInt ("Size");
	updateSize(0.0, 0.0);
}
PointInt& Node2d::getSize() {
	return getPropertyPointInt ("Size")->value;
}

//Scale
void Node2d::setScale(const PointFloat& r) {
	setPropertyPointFloat ("Scale", r);
	updateScale(r.x, r.y);
}
void Node2d::setScale() {
	setPropertyPointFloat ("Scale");
	updateScale(1.0, 1.0);
}
PointFloat& Node2d::getScale() {
	return getPropertyPointFloat ("Scale")->value;
}

//Rotation
void Node2d::setRotation(const float& r) {
	setPropertyFloat ("Rotation", r);
	updateRotation(r);
}
void Node2d::setRotation() {
	setPropertyFloat ("Rotation");
	updateRotation(0.0);
}
float& Node2d::getRotation() {
	return getPropertyFloat ("Rotation")->value;
}

//FlipX
void Node2d::setFlipX(const bool& r) {
	setPropertyBool ("FlipX", r);
	updateFlipX(r);
}

void Node2d::setFlipX() {
	setPropertyBool ("FlipX");
	updateFlipX(false);
}

bool& Node2d::getFlipX() {
	return getPropertyBool ("FlipX")->value;
}


//FlipY
void Node2d::setFlipY(const bool& r) {
	setPropertyBool ("FlipY", r);
	updateFlipY(r);
}
void Node2d::setFlipY() {
	setPropertyBool ("FlipY");
	updateFlipY(false);
}
bool& Node2d::getFlipY() {
	return getPropertyBool ("FlipY")->value;
}


//Visible
void Node2d::setVisible(const bool& r) {
	setPropertyBool ("Visible", r);
	updateVisible(r);
}

void Node2d::setVisible() {
	setPropertyBool ("Visible");
	updateVisible(true);
}

bool& Node2d::getVisible() {
	return getPropertyBool ("Visible")->value;
}

//FrameRef
void Node2d::setFrameRef(const FrameRef& r) {
	setPropertyFrameRef ("FrameRef", r);
	updateFrameRef(getFrameRef().resourcefile, getFrameRef().textureid, getFrameRef().frame);
}

void Node2d::setFrameRef() {
	setPropertyFrameRef ("FrameRef");
	updateFrameRef("", 0, "");
}

FrameRef& Node2d::getFrameRef() {
	return getPropertyFrameRef ("FrameRef")->value;
}


void Node2d::cleanup() {
	memset(&mDirtyInfo,0,sizeof(DirtyInfo));
	memset(&mSelectionInfo,0,sizeof(SelectionInfo));
	memset(&mTextureInfo,0,sizeof(TextureInfo));
	memset(&mLocationInfo,0,sizeof(LocationInfo));
	mLocationInfo.rModelMatrix=glm::mat4(1.0);
	deleteTexturedChildGroupList();
	mWiredChildGroup=deleteXWiredChildGroup(mWiredChildGroup);
	mHandleWiredChildGroup=deleteXWiredChildGroup(mHandleWiredChildGroup);
}

void Node2d::deleteTexturedChildGroupList() {
	if (mTexturedChildGroupList) {
		for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
			if (rTexturedChildGroup->rGlBufferId!=0) {
				glBindBufferARB(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &rTexturedChildGroup->rGlBufferId);
				rTexturedChildGroup->rGlBufferId=0;
			}
			if (rTexturedChildGroup->rVertexBuffer) {
				delete rTexturedChildGroup->rVertexBuffer;
				rTexturedChildGroup->rVertexBuffer=nullptr;
				rTexturedChildGroup->rCapacity=0;
				rTexturedChildGroup->rUsed=0;
			}
		}

		mTexturedChildGroupList->clear();
		delete mTexturedChildGroupList;
		mTexturedChildGroupList=nullptr;
	}
}
WiredChildGroup* Node2d::deleteXWiredChildGroup(WiredChildGroup *rWiredChildGroup) {
	if (rWiredChildGroup) {
		if (rWiredChildGroup->rGlBufferId!=0) {
			glBindBufferARB(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &rWiredChildGroup->rGlBufferId);
			rWiredChildGroup->rGlBufferId=0;
		}
		if (rWiredChildGroup->rVertexBuffer) {
			delete rWiredChildGroup->rVertexBuffer;
			rWiredChildGroup->rVertexBuffer=nullptr;
			rWiredChildGroup->rCapacity=0;
			rWiredChildGroup->rUsed=0;
		}
		delete rWiredChildGroup;
	}
	return nullptr;
}

void Node2d::updateVisible(bool rVisible) {
	if (mLocationInfo.rVisible!=rVisible) {
		mLocationInfo.rVisible=rVisible;
		mDirtyInfo.rVisible=true;
		if (mSelectionInfo.rIsSelected) {
			mSelectionInfo.rIsSelectionDirty=true;
		}
	}
}

void Node2d::updatePosition(float x, float y, float z) {
	if (mLocationInfo.rPosition.x!=x || mLocationInfo.rPosition.y!=y || mLocationInfo.rPosition.z!=z) {
		mLocationInfo.rPosition.x=x;
		mLocationInfo.rPosition.y=y;
		mLocationInfo.rPosition.z=z;
		mDirtyInfo.rPosition=true;
		if (mSelectionInfo.rIsSelected) {
			mSelectionInfo.rIsSelectionDirty=true;
		}
	}
}

void Node2d::updateSize(float x, float y) {
	if (mLocationInfo.rSize.x!=x || mLocationInfo.rSize.y!=y) {
		mLocationInfo.rSize.x=x;
		mLocationInfo.rSize.y=y;
		mDirtyInfo.rSize=true;
		if (mSelectionInfo.rIsSelected) {
			mSelectionInfo.rIsSelectionDirty=true;
		}
	}
}
void Node2d::updateScale(float x, float y) {
	if (mLocationInfo.rScale.x!=x || mLocationInfo.rScale.y!=y || mLocationInfo.rScale.z!=1.0) {
		mLocationInfo.rScale.x=x;
		mLocationInfo.rScale.y=y;
		mLocationInfo.rScale.z=1.0;
		mDirtyInfo.rScale=true;
		if (mSelectionInfo.rIsSelected) {
			mSelectionInfo.rIsSelectionDirty=true;
		}
	}
}

void Node2d::updateRotation(float rRotation) {
	if (mLocationInfo.rRotation!=rRotation) {
		mLocationInfo.rRotation=rRotation;
		mDirtyInfo.rRotation=true;
		if (mSelectionInfo.rIsSelected) {
			mSelectionInfo.rIsSelectionDirty=true;
		}
	}
}

void Node2d::updateFrameRef(const string&resourcefile, int textureid, const string&frame) {
	if (mFrameRefInfo.rFrameRef.resourcefile!=resourcefile || mFrameRefInfo.rFrameRef.textureid!=textureid ||
			mFrameRefInfo.rFrameRef.frame!=frame) {
		mFrameRefInfo.rFrameRef.resourcefile=resourcefile;
		mFrameRefInfo.rFrameRef.textureid=textureid;
		mFrameRefInfo.rFrameRef.frame=frame;
		//mFrameRefInfo.rIsFrameDirty=true;
		updateTextureAll();
	}
}

void Node2d::updateFlipX(bool rFlipX) {
	if (mLocationInfo.rFlipX!=rFlipX) {
		mLocationInfo.rFlipX=rFlipX;
		mDirtyInfo.rFlipX=true;
	}
}

void Node2d::updateFlipY(bool rFlipY) {
	if (mLocationInfo.rFlipY!=rFlipY) {
		mLocationInfo.rFlipY=rFlipY;
		mDirtyInfo.rFlipY=true;
	}
}

void Node2d::updateTextureAll() {
	NodeTexture *rNodeTexture=nullptr;
	NodeTextureFrame *rNodeTextureFrame=nullptr;
	BTexturePng *bTexture=nullptr;
	float tx=0.0;
	float ty=0.0;
	float tw=0.0;
	float th=0.0;

	TextureUtil::getTexture(this, &rNodeTexture, &rNodeTextureFrame);
	if (rNodeTexture) {
		bTexture=TextureUtil::getTexture(rNodeTexture->getPath());
		TextureUtil::getTextureCoords(this, rNodeTextureFrame, bTexture, &tx, &ty, &tw, &th);
	}

	updateTextureUV(tx, ty, tw, th);
	updateTexture(bTexture);
}

void Node2d::updateTextureUV(float x, float y, float u, float v) {
	if (mTextureInfo.rTextureXY.x!=x || mTextureInfo.rTextureXY.y!=y) {
		mTextureInfo.rTextureXY.x=x;
		mTextureInfo.rTextureXY.y=y;
		mDirtyInfo.rTextureUV=true;
	}
	if (mTextureInfo.rTextureWH.x!=u || mTextureInfo.rTextureWH.y!=v) {
		mTextureInfo.rTextureWH.x=u;
		mTextureInfo.rTextureWH.y=v;
		mDirtyInfo.rTextureUV=true;
	}
}

void Node2d::updateTexture(BTexturePng *rBTexturePng) {
	if (mTextureInfo.rBTexturePng!=rBTexturePng) {
		mTextureInfo.rBTexturePng=rBTexturePng;
		mDirtyInfo.rBTexturePng=true;
	}
}

void Node2d::updateSelection(bool isSelected) {
	if (mSelectionInfo.rIsSelected!=isSelected) {
		mSelectionInfo.rIsSelected=isSelected;
		mSelectionInfo.rIsSelectionDirty=true;
	}
}
#if 0
void Node2d::clearSelectionRecursive() {
	if (getChildCount()>0) {
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			rNode2d->updateSelection(false);
		}
	}
}
#endif

glm::mat4& Node2d::getModelMatrix() {
	return mLocationInfo.rModelMatrix;
}

glm::mat4& Node2d::getModelMatrixWithParents() {
	return mLocationInfo.rModelMatrixWithParent;
}

void Node2d::updateDraw(glm::mat4 &rViewMatrix) {
	updateMatrix(false);
	updateTextureVertexBuffer(false);
	updateSelectionVertexBuffer(false);
	draw(rViewMatrix);
	drawSelection(rViewMatrix);
}

void Node2d::updateMatrix(bool forceDirtyParent) {
	if (getChildCount()>0) {
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			// update matrix and local location
			if (forceDirtyParent || rNode2d->mDirtyInfo.isDirty()) {
				rNode2d->mLocationInfo.rModelMatrix=glm::mat4();
				if (rNode2d->mLocationInfo.rPosition.x!=0.0 || rNode2d->mLocationInfo.rPosition.y!=0.0) {
					rNode2d->mLocationInfo.rModelMatrix=glm::translate(rNode2d->mLocationInfo.rModelMatrix, rNode2d->mLocationInfo.rPosition);
				}
				if (rNode2d->mLocationInfo.rRotation!=0.0) {
					rNode2d->mLocationInfo.rModelMatrix=glm::rotate(rNode2d->mLocationInfo.rModelMatrix, glm::radians(rNode2d->mLocationInfo.rRotation), glm::vec3(0.0,0.0,1.0));
				}
				if (rNode2d->mLocationInfo.rFlipX || rNode2d->mLocationInfo.rFlipY || rNode2d->mLocationInfo.rScale.x!=1.0 || rNode2d->mLocationInfo.rScale.y!=1.0) {
					glm::vec3 scale=rNode2d->mLocationInfo.rScale;
					if (rNode2d->mLocationInfo.rFlipX) {
						scale.x=-scale.x;
					}
					if (rNode2d->mLocationInfo.rFlipY) {
						scale.y=-scale.y;
					}
					rNode2d->mLocationInfo.rModelMatrix=glm::scale(rNode2d->mLocationInfo.rModelMatrix, scale);
				}
				float w=rNode2d->mLocationInfo.rSize.x;
				float w2=w/2.0;
				float h=rNode2d->mLocationInfo.rSize.y;
				float h2=h/2.0;

				// **** Location relative to parent
				rNode2d->mLocationInfo.rLocalLocationCenter=mLocationInfo.rModelMatrix*glm::vec4{0.0, 0.0 ,0.0,1.0};
				// top left
				rNode2d->mLocationInfo.rVertexLocationBox[0]=glm::vec4{-w2,-h2,0.0,1.0};
				rNode2d->mLocationInfo.rLocalLocationBox[0]=rNode2d->mLocationInfo.rModelMatrix*rNode2d->mLocationInfo.rVertexLocationBox[0];
				// top right
				rNode2d->mLocationInfo.rVertexLocationBox[1]=glm::vec4{w2,-h2,0.0,1.0};
				rNode2d->mLocationInfo.rLocalLocationBox[1]=rNode2d->mLocationInfo.rModelMatrix*rNode2d->mLocationInfo.rVertexLocationBox[1];
				// bottom right
				rNode2d->mLocationInfo.rVertexLocationBox[2]=glm::vec4{w2,h2,0.0,1.0};
				rNode2d->mLocationInfo.rLocalLocationBox[2]=rNode2d->mLocationInfo.rModelMatrix*rNode2d->mLocationInfo.rVertexLocationBox[2];
				// bottom left
				rNode2d->mLocationInfo.rVertexLocationBox[3]=glm::vec4{-w2,h2,0.0,1.0};
				rNode2d->mLocationInfo.rLocalLocationBox[3]=rNode2d->mLocationInfo.rModelMatrix*rNode2d->mLocationInfo.rVertexLocationBox[3];
				// **** World Location
				rNode2d->mLocationInfo.rModelMatrixWithParent=mLocationInfo.rModelMatrixWithParent*rNode2d->mLocationInfo.rModelMatrix;

				rNode2d->mLocationInfo.rWorldLocationCenter=mLocationInfo.rModelMatrixWithParent*glm::vec4{float(rNode2d->mLocationInfo.rPosition.x),float(rNode2d->mLocationInfo.rPosition.y),0.0,1.0};
				// top left
				rNode2d->mLocationInfo.rWorldLocationBox[0]=rNode2d->mLocationInfo.rModelMatrixWithParent*rNode2d->mLocationInfo.rVertexLocationBox[0];
				// top right
				rNode2d->mLocationInfo.rWorldLocationBox[1]=rNode2d->mLocationInfo.rModelMatrixWithParent*rNode2d->mLocationInfo.rVertexLocationBox[1];
				// bottom right
				rNode2d->mLocationInfo.rWorldLocationBox[2]=rNode2d->mLocationInfo.rModelMatrixWithParent*rNode2d->mLocationInfo.rVertexLocationBox[2];
				// bottom left
				rNode2d->mLocationInfo.rWorldLocationBox[3]=rNode2d->mLocationInfo.rModelMatrixWithParent*rNode2d->mLocationInfo.rVertexLocationBox[3];

				rNode2d->mLocationInfo.rModelMatrixWithParentInverse=glm::inverse(rNode2d->mLocationInfo.rModelMatrixWithParent);

				float rResizeHandleSizeLocalX=10.0;
				float rResizeHandleSizeLocalY=10.0;
				for (int cnt=0;cnt<3;cnt++) {
					float startX=w2;
					float startY=h2;
					if (cnt==1) {
						startX=w2;//+rResizeHandleSizeLocalX;
						startY=0-rResizeHandleSizeLocalY/2.0;
					} else if (cnt==2) {
						startX=0-rResizeHandleSizeLocalX/2.0;
						startY=h2;//+rResizeHandleSizeLocalY;
					}

					for (int i=0;i<4;i++) {
						int x=startX;
						int y=startY;
						switch(i) {
						case 0:
							break;
						case 1:
							x+=rResizeHandleSizeLocalX;
							break;
						case 2:
							x+=rResizeHandleSizeLocalX;
							y+=rResizeHandleSizeLocalY;
							break;
						case 3:
							y+=rResizeHandleSizeLocalY;
							break;
						}
						rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i].x=x;
						rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i].y=y;
						rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i].z=0.0;
						rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i].w=1.0;
						rNode2d->mHandleLocationInfo[cnt].rLocalLocationBox[i]=rNode2d->mLocationInfo.rModelMatrix*rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i];
						rNode2d->mHandleLocationInfo[cnt].rWorldLocationBox[i]=rNode2d->mLocationInfo.rModelMatrixWithParent*rNode2d->mHandleLocationInfo[cnt].rVertexLocationBox[i];

					}
				}
			}
			rNode2d->updateMatrix(forceDirtyParent || rNode2d->mDirtyInfo.isDirty());
		}
	}
}
LocationInfo& Node2d::getLocationInfo() {
	return mLocationInfo;
}

glm::vec4 Node2d::getLocalPosFromWorldPos(float rWorldX, float rWorldY, bool rSnapToGrid) {
	glm::vec4 localXY(rWorldX, rWorldY, 0, 1);
	localXY=mLocationInfo.rModelMatrixWithParentInverse*localXY;
	if (rSnapToGrid) {
		snapToGrid(localXY);
	}
	return localXY;
}


float Node2d::calcGridPos(float worldPos, float gridSize) {
	float rv=worldPos;
	if (gridSize>0) {
		int gridField=0;
		if (worldPos>0) {
			gridField=(worldPos+gridSize/2)/gridSize;
		} else if (worldPos<0) {
			gridField=(worldPos-gridSize/2)/gridSize;
		}
		rv=gridField*gridSize;//+rGridState.gridX/2;
	}
	return rv;
}

void Node2d::snapToGrid(glm::vec4 &v) {
	int gridSizeX=TextureUtil::getGridSizeX();
	int gridSizeY=TextureUtil::getGridSizeY();
	v.x=calcGridPos(v.x, float(gridSizeX));
	v.y=calcGridPos(v.y, (float)gridSizeY);
}


void Node2d::updateTextureVertexBuffer(bool forceDirtyParent) {
	if (getChildCount()>0) {
		// count required box counts per texture
		bool haveDirty=false;
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			if (forceDirtyParent || rNode2d->mDirtyInfo.isDirty()) {
				haveDirty=true;
			}
			// increase texture use count
			if (rNode2d->mTextureInfo.rBTexturePng) {
				if (!mTexturedChildGroupList) {
					mTexturedChildGroupList=new list<TexturedChildGroup*>();
				}
				if (mTexturedChildGroupList) {
					TexturedChildGroup* rTexturedChildGroup_found=nullptr;
					for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
						if (rTexturedChildGroup->rBTexturePng==rNode2d->mTextureInfo.rBTexturePng) {
							rTexturedChildGroup_found=rTexturedChildGroup;
							break;
						}
					}
					if (!rTexturedChildGroup_found) {
						rTexturedChildGroup_found=new TexturedChildGroup();
						rTexturedChildGroup_found->rBTexturePng=rNode2d->mTextureInfo.rBTexturePng;
						mTexturedChildGroupList->push_back(rTexturedChildGroup_found);
					}
					if (rTexturedChildGroup_found) {
						rTexturedChildGroup_found->rAboutToUse++;
					}
				}

			}
		}
		bool refill=false;
		// build or reset buffer per texture
		if (mTexturedChildGroupList) {
			for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
				if (haveDirty || rTexturedChildGroup->rAboutToUse!=rTexturedChildGroup->rUsed) {
					refill=true;
					if (rTexturedChildGroup->rCapacity<rTexturedChildGroup->rAboutToUse) {
						if (rTexturedChildGroup->rVertexBuffer) {
							rTexturedChildGroup->rVertexBuffer=(TextureVertexBox*)realloc(rTexturedChildGroup->rVertexBuffer, sizeof(TextureVertexBox)*rTexturedChildGroup->rAboutToUse);
						} else {
							rTexturedChildGroup->rVertexBuffer=(TextureVertexBox*)malloc(sizeof(TextureVertexBox)*rTexturedChildGroup->rAboutToUse);
						}
						rTexturedChildGroup->rCapacity=rTexturedChildGroup->rAboutToUse;
					}
					if (rTexturedChildGroup->rVertexBuffer) {
						memset(rTexturedChildGroup->rVertexBuffer,0,sizeof(TextureVertexBox)*rTexturedChildGroup->rCapacity);
						rTexturedChildGroup->rUsed=rTexturedChildGroup->rAboutToUse;
					} else {
						rTexturedChildGroup->rUsed=0;
						rTexturedChildGroup->rCapacity=0;
					}
				}
				rTexturedChildGroup->rAboutToUse=0;
				rTexturedChildGroup->rNextToWriteIndex=0;
			}
		}
		if (refill) {
			for (Node* rNode : mNodes) {
				Node2d *rNode2d=static_cast<Node2d*>(rNode);
				if (rNode2d->mTextureInfo.rBTexturePng) {
					if (mTexturedChildGroupList) {
						TexturedChildGroup* rTexturedChildGroup_found=nullptr;
						for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
							if (rTexturedChildGroup->rBTexturePng==rNode2d->mTextureInfo.rBTexturePng) {
								rTexturedChildGroup_found=rTexturedChildGroup;
								break;
							}
						}
						if (rTexturedChildGroup_found) {
							for (int i=0;i<4;i++) {
								TextureVertex &rTextureVertex=rTexturedChildGroup_found->rVertexBuffer[rTexturedChildGroup_found->rNextToWriteIndex].textureVertex[i];
								glm::vec4 &s=rNode2d->mLocationInfo.rLocalLocationBox[i];
								rTextureVertex.wireVertex.x=s.x;
								rTextureVertex.wireVertex.y=s.y;
								rTextureVertex.wireVertex.z=s.z;
								if (i==0) {
									rTextureVertex.textureCoords.u=rNode2d->mTextureInfo.rTextureXY.x;
									rTextureVertex.textureCoords.v=rNode2d->mTextureInfo.rTextureXY.y;
								} else if (i==1) {
									rTextureVertex.textureCoords.u=rNode2d->mTextureInfo.rTextureWH.x;
									rTextureVertex.textureCoords.v=rNode2d->mTextureInfo.rTextureXY.y;
								} else if (i==2) {
									rTextureVertex.textureCoords.u=rNode2d->mTextureInfo.rTextureWH.x;
									rTextureVertex.textureCoords.v=rNode2d->mTextureInfo.rTextureWH.y;
								} else if (i==3) {
									rTextureVertex.textureCoords.u=rNode2d->mTextureInfo.rTextureXY.x;
									rTextureVertex.textureCoords.v=rNode2d->mTextureInfo.rTextureWH.y;
								}
							}
							rTexturedChildGroup_found->rNextToWriteIndex++;
						}
					}
				}
				//rNode2d->mDirtyInfo.resetDirty();
			}
		}
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			bool forceDirtyParent=forceDirtyParent||rNode2d->mDirtyInfo.isDirty();
			rNode2d->mDirtyInfo.resetDirty();
			//rNode2d->mFrameRefInfo.rIsFrameDirty=false;
			rNode2d->updateTextureVertexBuffer(forceDirtyParent);
		}
	}
}

bool Node2d::reallocateWiredVertexBufferIfRequired(WiredChildGroup* rWiredChildGroup, bool haveDirty) {
	bool refill=false;
	if (rWiredChildGroup) {
		if (haveDirty || rWiredChildGroup->rAboutToUse!=rWiredChildGroup->rUsed) {
			refill=true;
			if (rWiredChildGroup->rCapacity<rWiredChildGroup->rAboutToUse) {
				if (rWiredChildGroup->rVertexBuffer) {
					rWiredChildGroup->rVertexBuffer=(WireVertexBox*)realloc(rWiredChildGroup->rVertexBuffer, sizeof(WireVertexBox)*rWiredChildGroup->rAboutToUse);
				} else {
					rWiredChildGroup->rVertexBuffer=(WireVertexBox*)malloc(sizeof(WireVertexBox)*rWiredChildGroup->rAboutToUse);
				}
				rWiredChildGroup->rCapacity=rWiredChildGroup->rAboutToUse;
			}
			memset(rWiredChildGroup->rVertexBuffer,0,sizeof(sizeof(WireVertexBox)*rWiredChildGroup->rCapacity));
			rWiredChildGroup->rUsed=rWiredChildGroup->rAboutToUse;
		}
		rWiredChildGroup->rAboutToUse=0;
		rWiredChildGroup->rNextToWriteIndex=0;
	}
	return refill;
}

void Node2d::updateSelectionVertexBuffer(bool forceDirtyParent) {
	if (getChildCount()>0) {
		bool haveDirty=false;
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			if (forceDirtyParent || rNode2d->mSelectionInfo.rIsSelectionDirty) {
				haveDirty=true;
			}
			if (rNode2d->mSelectionInfo.rIsSelected) {
				if (!mWiredChildGroup) {
					mWiredChildGroup=new WiredChildGroup();
				}
				mWiredChildGroup->rAboutToUse++;

				if (!mHandleWiredChildGroup) {
					mHandleWiredChildGroup=new WiredChildGroup();
				}
				mHandleWiredChildGroup->rAboutToUse+=3;
			}
		}
		// build or reset vertext buffer
		bool refill=reallocateWiredVertexBufferIfRequired(mWiredChildGroup, haveDirty);
		if (mWiredChildGroup && refill) {
			for (Node* rNode : mNodes) {
				Node2d *rNode2d=static_cast<Node2d*>(rNode);
				if (rNode2d->mSelectionInfo.rIsSelected) {
					for (int i=0;i<4;i++) {
						WireVertex &rWireVertex=mWiredChildGroup->rVertexBuffer[mWiredChildGroup->rNextToWriteIndex].wireVertex[i];
						glm::vec4 &s=rNode2d->mLocationInfo.rLocalLocationBox[i];
						rWireVertex.x=s.x;
						rWireVertex.y=s.y;
						rWireVertex.z=s.z;
					}
					mWiredChildGroup->rNextToWriteIndex++;
				}
				//rNode2d->mDirtyInfo.resetDirty();
			}
		}
		bool refillHandle=reallocateWiredVertexBufferIfRequired(mHandleWiredChildGroup, haveDirty);
		if (mHandleWiredChildGroup && refillHandle) {
			for (Node* rNode : mNodes) {
				Node2d *rNode2d=static_cast<Node2d*>(rNode);
				if (rNode2d->mSelectionInfo.rIsSelected) {
					for (int cnt=0;cnt<3;cnt++) {
						for (int i=0;i<4;i++) {
							WireVertex &rWireVertex=mHandleWiredChildGroup->rVertexBuffer[mHandleWiredChildGroup->rNextToWriteIndex].wireVertex[i];
							glm::vec4 &s=rNode2d->mHandleLocationInfo[cnt].rLocalLocationBox[i];
							rWireVertex.x=s.x;
							rWireVertex.y=s.y;
							rWireVertex.z=s.z;
						}
						mHandleWiredChildGroup->rNextToWriteIndex++;
					}
				}
				//rNode2d->mDirtyInfo.resetDirty();
			}
		}
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			bool forceDirtyParent=forceDirtyParent || rNode2d->mSelectionInfo.rIsSelectionDirty;
			rNode2d->mSelectionInfo.rIsSelectionDirty=false;
			rNode2d->updateSelectionVertexBuffer(forceDirtyParent);
		}
	}
}


void Node2d::draw(glm::mat4 &rViewMatrix) {
	if (getChildCount()>0 && getVisible()) {
		// reset required box counts
		if (mTexturedChildGroupList) {
			glLoadMatrixf (glm::value_ptr(rViewMatrix*mLocationInfo.rModelMatrixWithParent));
			for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
				if (rTexturedChildGroup->rUsed>0) {
					if (rTexturedChildGroup->rGlBufferId==0) {
						glGenBuffers(1, &rTexturedChildGroup->rGlBufferId);
					}
					if (rTexturedChildGroup->rGlBufferId!=0) {
						glEnable(GL_TEXTURE_2D);
						glBindBuffer(GL_ARRAY_BUFFER, rTexturedChildGroup->rGlBufferId);
						rTexturedChildGroup->rBTexturePng->bind();
						//glPolygonMode(GL_FRONT, GL_FILL);
						//glPolygonMode(GL_BACK, GL_FILL);
						glColor3f(1.0,1.0, 1.0);
						glInterleavedArrays(GL_T2F_V3F, sizeof(TextureVertex), nullptr);
						glBufferData(GL_ARRAY_BUFFER, rTexturedChildGroup->rUsed*sizeof(TextureVertexBox), rTexturedChildGroup->rVertexBuffer, GL_STATIC_DRAW);
						glDrawArrays(GL_QUADS, 0, rTexturedChildGroup->rUsed*4);
					}
				}
			}
		}
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			rNode2d->draw(rViewMatrix);
		}
	}
}

void Node2d::drawSelection(glm::mat4 &rViewMatrix) {
	if (getChildCount()>0 && getVisible()) {
		// reset required box counts
		bool rDrawSelection=false;
		bool rDrawHandles=false;
		if (mWiredChildGroup && mWiredChildGroup->rUsed>0) {
			rDrawSelection=true;
		}
		if (mHandleWiredChildGroup && mHandleWiredChildGroup->rUsed>0) {
			rDrawHandles=true;
		}
		if (rDrawSelection || rDrawHandles) {
			glDisable(GL_TEXTURE_2D);
			glLoadMatrixf (glm::value_ptr(rViewMatrix*mLocationInfo.rModelMatrixWithParent));
			if (rDrawSelection && mWiredChildGroup->rGlBufferId==0) {
				glGenBuffers(1, &mWiredChildGroup->rGlBufferId);
			}
			if (rDrawSelection && mWiredChildGroup->rGlBufferId!=0) {
				glBindBuffer(GL_ARRAY_BUFFER, mWiredChildGroup->rGlBufferId);
				glLineWidth(2.0);
				glLineStipple(1, 0x3FC9);
				glEnable(GL_LINE_STIPPLE);
				glPolygonMode(GL_FRONT, GL_LINE);
				glPolygonMode(GL_BACK, GL_LINE);
				glColor3f(0.3, 0.5, 1.0);
				glInterleavedArrays(GL_V3F, sizeof(WireVertex), nullptr);
				glBufferData(GL_ARRAY_BUFFER, mWiredChildGroup->rUsed*sizeof(WireVertexBox), mWiredChildGroup->rVertexBuffer, GL_STATIC_DRAW);
				glDrawArrays(GL_QUADS, 0, mWiredChildGroup->rUsed*4);
				glDisable(GL_LINE_STIPPLE);
			}
			if (rDrawHandles && mHandleWiredChildGroup->rGlBufferId==0) {
				glGenBuffers(1, &mHandleWiredChildGroup->rGlBufferId);
			}
			if (rDrawHandles && mHandleWiredChildGroup->rGlBufferId!=0) {
				glBindBuffer(GL_ARRAY_BUFFER, mHandleWiredChildGroup->rGlBufferId);
				glLineWidth(2.0);
				//glLineStipple(1, 0x3FC9);
				//glEnable(GL_LINE_STIPPLE);
				glPolygonMode(GL_FRONT, GL_LINE);
				glPolygonMode(GL_BACK, GL_LINE);
				glColor3f(0.3,1.5, 1.0);
				glInterleavedArrays(GL_V3F, sizeof(WireVertex), nullptr);
				glBufferData(GL_ARRAY_BUFFER, mHandleWiredChildGroup->rUsed*sizeof(WireVertexBox), mHandleWiredChildGroup->rVertexBuffer, GL_STATIC_DRAW);
				glDrawArrays(GL_QUADS, 0, mHandleWiredChildGroup->rUsed*4);
			}
		}
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			rNode2d->drawSelection(rViewMatrix);
		}
	}
}

static bool intersects(float x,float y,float dx,float dy,float x1,float y1,float dx1,float dy1) {
	if (x>dx) std::swap(x,dx);
	if (y>dy) std::swap(y,dy);
	if (x1>dx1) std::swap(x1,dx1);
	if (y1>dy1) std::swap(y1,dy1);
	float left   = std::max(x, x1);
	float top    = std::max(y, y1);
	float right  = std::min(dx, dx1);
	float bottom = std::min(dy, dy1);

	if ((left <= right) && (top <= bottom)) {
		//intersection = Rect<T>(left, top, right - left, bottom - top);
		return true;
	} else {
		//intersection = Rect<T>(0, 0, 0, 0);
		return false;
	}
}

static bool intersects2(float x,float y,float dx,float dy,float testX, float testY) {
	if (x>dx) {
		float tmp=x;
		x=dx;
		dx=tmp;
	}
	if (y>dy) {
		float tmp=y;
		y=dy;
		dy=tmp;
	}
	if (testX>=x && testX<=dx && testY>=y && testY<=dy) {
		//std::cout << "intersect: " << testX <<" x " << testY << " is inside x/y " << x << " x " << y << " and dx/dy " <<dx <<" x " << dy << std::endl;
		return true;
	}
	return false;
}

static bool isBoxIntersecting(HandleLocationInfo *rHandleLocationInfo, float worldX, float worldY, float worldDX, float worldDY) {
	bool rv=false;
	vector<PointFloat> v1;
	vector<PointFloat> v2;
	v2.emplace_back(worldX, worldY);
	v2.emplace_back(worldX+worldDX, worldY);
	v2.emplace_back(worldX+worldDX, worldY+worldDY);
	v2.emplace_back(worldX, worldY+worldDY);
	v2.emplace_back(worldX, worldY);
	v1.emplace_back(rHandleLocationInfo->rWorldLocationBox[0].x,rHandleLocationInfo->rWorldLocationBox[0].y );
	v1.emplace_back(rHandleLocationInfo->rWorldLocationBox[1].x,rHandleLocationInfo->rWorldLocationBox[1].y);
	v1.emplace_back(rHandleLocationInfo->rWorldLocationBox[2].x,rHandleLocationInfo->rWorldLocationBox[2].y);
	v1.emplace_back(rHandleLocationInfo->rWorldLocationBox[3].x,rHandleLocationInfo->rWorldLocationBox[3].y);
	v1.emplace_back(rHandleLocationInfo->rWorldLocationBox[0].x,rHandleLocationInfo->rWorldLocationBox[0].y);
	if (Intersection::isPolygonsIntersecting(v1, v2)) {
		rv=true;
	}
	return rv;
}

HandleLocationInfo* Node2d::intersectsHandle(float worldX, float worldY, HandleLocation::HandleNumber rHandleNumber) {
	if (getNodeType()==NodeType::Sprite && getVisible() && mSelectionInfo.rIsSelected) {
		if (isBoxIntersecting(&mHandleLocationInfo[rHandleNumber], worldX, worldY, 1.0, 1.0)) {
			return &mHandleLocationInfo[rHandleNumber];
		}
	}
	return nullptr;
}

void Node2d::pickUpBox(vector<Node2d*> &v, float x, float y, float dx, float dy) {
	if (getNodeType()==NodeType::Sprite && getVisible()) {
		bool ss=false;
		for (int i=0;i<4;i++) {
			if (intersects2(x, y, dx, dy,
							mLocationInfo.rWorldLocationBox[i].x, mLocationInfo.rWorldLocationBox[i].y)) {
				ss=true;
				break;
			}
		}
		if (!ss) {
			glm::vec4 local[4];
			local[0].x=x;
			local[0].y=y;
			local[0].z=0;
			local[0].w=1.0;

			local[1].x=dx;
			local[1].y=y;
			local[1].z=0;
			local[1].w=1.0;

			local[2].x=dx;
			local[2].y=dy;
			local[2].z=0;
			local[2].w=1.0;

			local[3].x=x;
			local[3].y=dy;
			local[3].z=0;
			local[3].w=1.0;

			for (int i=0;i<4;i++) {
				local[i]=mLocationInfo.rModelMatrixWithParentInverse*local[i];
				if (intersects2(mLocationInfo.rVertexLocationBox[0].x, mLocationInfo.rVertexLocationBox[0].y, mLocationInfo.rVertexLocationBox[2].x, mLocationInfo.rVertexLocationBox[2].y,
								local[i].x, local[i].y)) {
					ss=true;
					break;
				}
				//std::cout << "Local " << std::to_string(i) <<" " << local[i].x << " x " << local[i].y << std::endl;
			}
		}

		if (ss) {
			v.push_back(this);
		}
	}
	if (getVisible() && getChildCount()>0) {
		for (Node* rNode : mNodes) {
			Node2d *rNode2d=static_cast<Node2d*>(rNode);
			rNode2d->pickUpBox(v, x, y, dx, dy);
		}
	}
}






void Node2d::setResizeHandleSizeLocal(float rResizeHandleSizeLocalX, float rResizeHandleSizeLocalY) {
	glm::mat4 matrix=glm::make_mat4x4(mCurrentModelMatrix.getPointer());

	int w=getSize().x;
	float w2=w/2.0;
	int h=getSize().y;
	float h2=h/2.0;

	for (int cnt=0;cnt<3;cnt++) {
		float startX=w2;
		float startY=h2;
		if (cnt==1) {
			startX=w2;//+rResizeHandleSizeLocalX;
			startY=0-rResizeHandleSizeLocalY/2.0;
		} else if (cnt==2) {
			startX=0-rResizeHandleSizeLocalX/2.0;
			startY=h2;//+rResizeHandleSizeLocalY;
		}

		for (int i=0;i<4;i++) {
			int x=startX;
			int y=startY;
			switch(i) {
			case 0:
				break;
			case 1:
				x+=rResizeHandleSizeLocalX;
				break;
			case 2:
				x+=rResizeHandleSizeLocalX;
				y+=rResizeHandleSizeLocalY;
				break;
			case 3:
				y+=rResizeHandleSizeLocalY;
				break;
			}
			glm::vec4 vh=matrix*glm::vec4{x,y,0.0,1.0};
			if (cnt==0) {
				mResizeHandleBR[i].x=vh.x;
				mResizeHandleBR[i].y=vh.y;
				mResizeHandleLocalBR[i].x=x;
				mResizeHandleLocalBR[i].y=y;
			} else if (cnt==1) {
				mResizeHandleRight[i].x=vh.x;
				mResizeHandleRight[i].y=vh.y;
				mResizeHandleLocalRight[i].x=x;
				mResizeHandleLocalRight[i].y=y;
			} else if (cnt==2) {
				mResizeHandleBottom[i].x=vh.x;
				mResizeHandleBottom[i].y=vh.y;
				mResizeHandleLocalBottom[i].x=x;
				mResizeHandleLocalBottom[i].y=y;
			}
		}
	}
}
#if 0
void Node2d::setCurrentLocalModelMatrix(GLMMatrix4 &m) {
	mCurrentLocalModelMatrix.setFromPointer(m.getPointer());
	calculateCoords(mCurrentLocalModelMatrix, &mCurrentLocalLocationCenter, &mCurrentLocalLocationBox[0]);
}

GLMMatrix4 Node2d::getCurrentLocalModelMatrix() {
	return mCurrentLocalModelMatrix;
}

void Node2d::setCurrentModelMatrix(GLMMatrix4 &m) {
	mCurrentModelMatrix.setFromPointer(m.getPointer());
	calculateCoords(mCurrentModelMatrix, &mCurrentWorldLocationCenter, &mCurrentWorldLocationBox[0]);
}

GLMMatrix4 Node2d::getCurrentModelMatrix() {
	return mCurrentModelMatrix;
}

void Node2d::calculateCoords(GLMMatrix4 &m, PointFloat* current2LocationCenter, PointFloat* mCurrent4LocationBox) {
	glm::mat4 matrix=glm::make_mat4x4(m.getPointer());

	int w=getSize().x;
	float w2=w/2.0;
	int h=getSize().y;
	float h2=h/2.0;

	glm::vec4 rCenter=matrix*glm::vec4{0.0,0.0,0.0,1.0};
	current2LocationCenter->x=rCenter.x;
	current2LocationCenter->y=rCenter.y;

	// top left
	glm::vec4 rW=matrix*glm::vec4{-w2,-h2,0.0,1.0};
	mCurrent4LocationBox[0].x=rW.x;
	mCurrent4LocationBox[0].y=rW.y;
	// top right
	rW=matrix*glm::vec4{w2,-h2,0.0,1.0};
	mCurrent4LocationBox[1].x=rW.x;
	mCurrent4LocationBox[1].y=rW.y;
	// bottom right
	rW=matrix*glm::vec4{w2,h2,0.0,1.0};
	mCurrent4LocationBox[2].x=rW.x;
	mCurrent4LocationBox[2].y=rW.y;
	// bottom left
	rW=matrix*glm::vec4{-w2,h2,0.0,1.0};
	mCurrent4LocationBox[3].x=rW.x;
	mCurrent4LocationBox[3].y=rW.y;
}
#endif
