#include "drawnode.h"
#if 0


DrawNode::DrawNode(Node2d *rNode2d)
{
	mNode2d=rNode2d;
}

DrawNode::~DrawNode() {
	cleanup();
}

void DrawNode::cleanup() {
	memset(&mDirtyInfo,0,sizeof(DirtyInfo));
	memset(&mSelectionInfo,0,sizeof(SelectionInfo));
	memset(&mTextureInfo,0,sizeof(TextureInfo));
	memset(&mLocationInfo,0,sizeof(LocationInfo));
	mLocationInfo.rModelMatrix=glm::mat4(1.0);
	deleteTexturedChildGroupList();
	deleteWiredChildGroup();
}



void DrawNode::updateSelectionRecursive(void *rUserData, bool isSelected) {
	if (mUserData==rUserData) {
		updateSelection(isSelected);
	} else {
		if (mChildList) {
			for (DrawNode* rDrawNode : *mChildList) {
				rDrawNode->updateSelectionRecursive(rUserData, isSelected);
			}
		}
	}
}

glm::mat4& DrawNode::getModelMatrix() {
	return mLocationInfo.rModelMatrix;
}

void DrawNode::updateSelectionVertexBuffer() {
	if (mChildList) {
		bool haveDirty=false;
		for (DrawNode* rDrawNode : *mChildList) {
			if (rDrawNode->mSelectionInfo.rIsSelectionDirty) {
				haveDirty=true;
			}
			if (rDrawNode->mSelectionInfo.rIsSelected) {
				if (!mWiredChildGroup) {
					mWiredChildGroup=new WiredChildGroup();
				}
				mWiredChildGroup->rAboutToUse++;
			}
		}
		bool refill=false;
		// build or reset buffer per texture
		if (mWiredChildGroup) {
			if (haveDirty || mWiredChildGroup->rAboutToUse!=mWiredChildGroup->rUsed) {
				refill=true;
				if (mWiredChildGroup->rCapacity<mWiredChildGroup->rAboutToUse) {
					if (mWiredChildGroup->rVertexBuffer) {
						mWiredChildGroup->rVertexBuffer=(WireVertexBox*)realloc(mWiredChildGroup->rVertexBuffer, sizeof(WireVertexBox)*mWiredChildGroup->rAboutToUse);
					} else {
						mWiredChildGroup->rVertexBuffer=(WireVertexBox*)malloc(sizeof(WireVertexBox)*mWiredChildGroup->rAboutToUse);
					}
					mWiredChildGroup->rCapacity=mWiredChildGroup->rAboutToUse;
				}
				memset(mWiredChildGroup->rVertexBuffer,0,sizeof(sizeof(WireVertexBox)*mWiredChildGroup->rCapacity));
				mWiredChildGroup->rUsed=mWiredChildGroup->rAboutToUse;
			}
			mWiredChildGroup->rAboutToUse=0;
			mWiredChildGroup->rNextToWriteIndex=0;
		}
		if (mWiredChildGroup && refill) {
			for (DrawNode* rDrawNode : *mChildList) {
				if (rDrawNode->mSelectionInfo.rIsSelected) {
					for (int i=0;i<4;i++) {
						WireVertex &rWireVertex=mWiredChildGroup->rVertexBuffer[mWiredChildGroup->rNextToWriteIndex].wireVertex[i];
						glm::vec4 &s=rDrawNode->mLocationInfo.rLocalLocationBox[i];
						rWireVertex.x=s.x;
						rWireVertex.y=s.y;
						rWireVertex.z=s.z;
					}
					mWiredChildGroup->rNextToWriteIndex++;
				}
				//rDrawNode->mDirtyInfo.resetDirty();
			}
		}
		for (DrawNode* rDrawNode : *mChildList) {
			rDrawNode->mSelectionInfo.rIsSelectionDirty=false;
			rDrawNode->updateSelectionVertexBuffer();
		}
	}
}

void DrawNode::updateTextureVertexBuffer() {
	if (mChildList) {
		// count required box counts per texture
		bool haveDirty=false;
		for (DrawNode* rDrawNode : *mChildList) {
			// update matrix and local location
			if (rDrawNode->mDirtyInfo.isDirty()) {
				haveDirty=true;
				rDrawNode->mLocationInfo.rModelMatrix=glm::mat4();
				if (rDrawNode->mLocationInfo.rPosition.x!=0.0 || rDrawNode->mLocationInfo.rPosition.y!=0.0) {
					rDrawNode->mLocationInfo.rModelMatrix=glm::translate(rDrawNode->mLocationInfo.rModelMatrix, rDrawNode->mLocationInfo.rPosition);
				}
				if (rDrawNode->mLocationInfo.rRotation!=0.0) {
					rDrawNode->mLocationInfo.rModelMatrix=glm::rotate(rDrawNode->mLocationInfo.rModelMatrix, glm::radians(rDrawNode->mLocationInfo.rRotation), glm::vec3(0.0,0.0,1.0));
				}
				if (rDrawNode->mLocationInfo.rScale.x!=1.0 || rDrawNode->mLocationInfo.rScale.y!=1.0) {
					rDrawNode->mLocationInfo.rModelMatrix=glm::scale(rDrawNode->mLocationInfo.rModelMatrix, rDrawNode->mLocationInfo.rScale);
				}
				float w=rDrawNode->mLocationInfo.rSize.x;
				float w2=w/2.0;
				float h=rDrawNode->mLocationInfo.rSize.y;
				float h2=h/2.0;

				rDrawNode->mLocationInfo.rLocalLocationCenter=mLocationInfo.rModelMatrix*glm::vec4{0.0,0.0,0.0,1.0};
				// top left
				rDrawNode->mLocationInfo.rVertexLocationBox[0]=glm::vec4{-w2,-h2,0.0,1.0};
				rDrawNode->mLocationInfo.rLocalLocationBox[0]=rDrawNode->mLocationInfo.rModelMatrix*rDrawNode->mLocationInfo.rVertexLocationBox[0];
				// top right
				rDrawNode->mLocationInfo.rVertexLocationBox[1]=glm::vec4{w2,-h2,0.0,1.0};
				rDrawNode->mLocationInfo.rLocalLocationBox[1]=rDrawNode->mLocationInfo.rModelMatrix*rDrawNode->mLocationInfo.rVertexLocationBox[1];
				// bottom right
				rDrawNode->mLocationInfo.rVertexLocationBox[2]=glm::vec4{w2,h2,0.0,1.0};
				rDrawNode->mLocationInfo.rLocalLocationBox[2]=rDrawNode->mLocationInfo.rModelMatrix*rDrawNode->mLocationInfo.rVertexLocationBox[2];
				// bottom left
				rDrawNode->mLocationInfo.rVertexLocationBox[3]=glm::vec4{-w2,h2,0.0,1.0};
				rDrawNode->mLocationInfo.rLocalLocationBox[3]=rDrawNode->mLocationInfo.rModelMatrix*rDrawNode->mLocationInfo.rVertexLocationBox[3];
			}
			// increase texture use count
			if (rDrawNode->mTextureInfo.rBTexturePng) {
				if (!mTexturedChildGroupList) {
					mTexturedChildGroupList=new list<TexturedChildGroup*>();
				}
				if (mTexturedChildGroupList) {
					TexturedChildGroup* rTexturedChildGroup_found=nullptr;
					for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
						if (rTexturedChildGroup->rBTexturePng==rDrawNode->mTextureInfo.rBTexturePng) {
							rTexturedChildGroup_found=rTexturedChildGroup;
							break;
						}
					}
					if (!rTexturedChildGroup_found) {
						rTexturedChildGroup_found=new TexturedChildGroup();
						rTexturedChildGroup_found->rBTexturePng=rDrawNode->mTextureInfo.rBTexturePng;
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
			for (DrawNode* rDrawNode : *mChildList) {
				if (rDrawNode->mTextureInfo.rBTexturePng) {
					if (mTexturedChildGroupList) {
						TexturedChildGroup* rTexturedChildGroup_found=nullptr;
						for (TexturedChildGroup* rTexturedChildGroup : *mTexturedChildGroupList) {
							if (rTexturedChildGroup->rBTexturePng==rDrawNode->mTextureInfo.rBTexturePng) {
								rTexturedChildGroup_found=rTexturedChildGroup;
								break;
							}
						}
						if (rTexturedChildGroup_found) {
							for (int i=0;i<4;i++) {
								TextureVertex &rTextureVertex=rTexturedChildGroup_found->rVertexBuffer[rTexturedChildGroup_found->rNextToWriteIndex].textureVertex[i];
								glm::vec4 &s=rDrawNode->mLocationInfo.rLocalLocationBox[i];
								rTextureVertex.wireVertex.x=s.x;
								rTextureVertex.wireVertex.y=s.y;
								rTextureVertex.wireVertex.z=s.z;
								if (i==0) {
									rTextureVertex.textureCoords.u=rDrawNode->mTextureInfo.rTextureXY.x;
									rTextureVertex.textureCoords.v=rDrawNode->mTextureInfo.rTextureXY.y;
								} else if (i==1) {
									rTextureVertex.textureCoords.u=rDrawNode->mTextureInfo.rTextureWH.x;
									rTextureVertex.textureCoords.v=rDrawNode->mTextureInfo.rTextureXY.y;
								} else if (i==2) {
									rTextureVertex.textureCoords.u=rDrawNode->mTextureInfo.rTextureWH.x;
									rTextureVertex.textureCoords.v=rDrawNode->mTextureInfo.rTextureWH.y;
								} else if (i==3) {
									rTextureVertex.textureCoords.u=rDrawNode->mTextureInfo.rTextureXY.x;
									rTextureVertex.textureCoords.v=rDrawNode->mTextureInfo.rTextureWH.y;
								}
							}
							rTexturedChildGroup_found->rNextToWriteIndex++;
						}
					}
				}
				//rDrawNode->mDirtyInfo.resetDirty();
			}
		}
		for (DrawNode* rDrawNode : *mChildList) {
			rDrawNode->mDirtyInfo.resetDirty();
			rDrawNode->updateTextureVertexBuffer();
		}
	}
}

void DrawNode::draw(glm::mat4 &rParentMatrix) {
	if (mChildList) {
		// reset required box counts
		if (mTexturedChildGroupList) {
			glLoadMatrixf (glm::value_ptr(rParentMatrix));
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
		for (DrawNode* rDrawNode : *mChildList) {
			glm::mat4 m=rParentMatrix*rDrawNode->mLocationInfo.rModelMatrix;
			rDrawNode->draw(m);
		}
	}
}

void DrawNode::drawSelection(glm::mat4 &rParentMatrix) {
	if (mChildList) {
		// reset required box counts
		if (mWiredChildGroup) {
			if (mWiredChildGroup->rUsed>0) {
				glDisable(GL_TEXTURE_2D);
				glLoadMatrixf (glm::value_ptr(rParentMatrix));
				if (mWiredChildGroup->rGlBufferId==0) {
					glGenBuffers(1, &mWiredChildGroup->rGlBufferId);
				}
				if (mWiredChildGroup->rGlBufferId!=0) {
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
				}
			}
		}
		for (DrawNode* rDrawNode : *mChildList) {
			glm::mat4 m=rParentMatrix*rDrawNode->mLocationInfo.rModelMatrix;
			rDrawNode->drawSelection(m);
		}
	}
}

void DrawNode::pickUp(vector<DrawNode*> &v, float localToParentX, float localToParentY) {
	if (mChildList) {
		for (DrawNode* rDrawNode : *mChildList) {
			if (
				localToParentX >= rDrawNode->mLocationInfo.rLocalLocationBox[0].x &&
				localToParentX <= rDrawNode->mLocationInfo.rLocalLocationBox[1].x &&
				localToParentY >= rDrawNode->mLocationInfo.rLocalLocationBox[0].y &&
				localToParentY <= rDrawNode->mLocationInfo.rLocalLocationBox[3].y) {
				v.push_back(rDrawNode);
			}
		}
		glm::mat4 inverseModelMatrix=glm::inverse(this->mLocationInfo.rModelMatrix);
		glm::vec4 vLocal(localToParentX, localToParentY, 0, 1);
		vLocal=inverseModelMatrix*vLocal;
		for (DrawNode* rDrawNode : *mChildList) {
			rDrawNode->pickUp(v, vLocal.x, vLocal.y);
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

	if ((left < right) && (top < bottom)) {
		//intersection = Rect<T>(left, top, right - left, bottom - top);
		return true;
	} else {
		//intersection = Rect<T>(0, 0, 0, 0);
		return false;
	}
}

void DrawNode::pickUpBox(glm::mat4 rParentsModelMatrix, vector<DrawNode*> &v, float x, float y, float dx, float dy) {
	rParentsModelMatrix*=mLocationInfo.rModelMatrix;
	glm::mat4 inverseModelMatrix=glm::inverse(rParentsModelMatrix);
	glm::vec4 localX(x, y, 0, 1);
	localX=inverseModelMatrix*localX;
	glm::vec4 localDX(dx, dy, 0, 1);
	localDX=inverseModelMatrix*localDX;
	if (intersects(mLocationInfo.rVertexLocationBox[0].x,
				  mLocationInfo.rVertexLocationBox[0].y,
				  mLocationInfo.rVertexLocationBox[2].x,
				  mLocationInfo.rVertexLocationBox[2].y,
				  localX.x, localX.y, localDX.x, localDX.y)) {
		v.push_back(this);
	}
	if (mChildList) {
		for (DrawNode* rDrawNode : *mChildList) {
			rDrawNode->pickUpBox(rParentsModelMatrix, v, x, y, dx, dy);
		}
	}
}
#endif
