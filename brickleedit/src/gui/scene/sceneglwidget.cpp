#include "sceneglwidget.h"
#include "../guicontext.h"
#include "../../project/projectcontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "worldcalculator.h"

void SceneGlWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0,0,0,1);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//texture= new QOpenGLTexture(QImage("data/assets/test.bmp"));

	//bTexture.load("data/assets/gravity.png");
#if 0
	glEnable(GL_TEXTURE_2D);
	unsigned error = lodepng::decode(image, lwidth, lheight, "data/assets/testhg.png");
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lwidth, lheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lwidth, lheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
#if 0
	QPainter p;
	p.begin(this);
	p.drawText(100,100,"Hallo Welt");
	p.end();
#endif

}

void SceneGlWidget::updateViewport() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float w=static_cast<float>(mCamera.getViewportWidth());
	float h=float(mCamera.getViewportHeight());
	float orthoLeft=(-w/2.0);
	float orthoRight=(w/2.0);
	float orthoBottom=(h/2.0);
	float orthoTop=(-h/2.0);
	glOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, 0.0f, 1.0f);

	GLfloat matrixArray[16];
	glGetFloatv (GL_PROJECTION_MATRIX, matrixArray);
	GLMMatrix4 pr;
	pr.setFromPointer(matrixArray);
	mCamera.setProjectionMatrix(pr);
	glMatrixMode(GL_MODELVIEW);
}


Camera& SceneGlWidget::getCamera() {
	return mCamera;
}

void SceneGlWidget::wheelEvent(QWheelEvent *event){
	int zoomUnits=0;
	if (event->delta()>0) {
		zoomUnits=-2;
	} else {
		zoomUnits=2;
	}
	zoomInOut(zoomUnits);
}

void SceneGlWidget::zoomInOut(int units) {
	if (units!=0) {
		mCamera.move(0,0,units);
		GuiContext::getInstance().onZoomLevelChanged(mCamera.getZ());
		this->update();//update();
	}
}

void SceneGlWidget::mousePressEvent(QMouseEvent * event ) {
	if (event->button()==Qt::MiddleButton) {
		cout << "Middle Mouse pressed" << std::endl;
		mViewportMoveInfo.isOnMove=true;
		mViewportMoveInfo.startX=event->pos().x();
		mViewportMoveInfo.startY=event->pos().y();
	} else if (event->button()==Qt::LeftButton) {
		onLeftMouseClicked(event, event->pos().x(), event->pos().y());
	}
}

void SceneGlWidget::mouseReleaseEvent(QMouseEvent * event ) {
	if (event->button()==Qt::MiddleButton) {
		mViewportMoveInfo.isOnMove=false;
		mViewportMoveInfo.startX=0;
		mViewportMoveInfo.startY=0;
	} else 	if (event->button()==Qt::LeftButton) {
		if (mSceneItemMoveManager.isOnMove()) {
			vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
			if (!v.empty()) {
				Node *latestSelected=v.back();
				GuiContext::getInstance().setSceneNodeInTreeAsSelected(latestSelected);
			}
		}
		mSceneItemMoveManager.stopMove();
		if (mSceneItemResizeManager.isOnResize()) {
			GuiContext::getInstance().setSceneNodeInTreeAsSelected(mSceneItemResizeManager.getNodeForResize());
			mSceneItemResizeManager.stopResize();
		}
		if (mSceneBulkSelectManager.isOnSelect()) {
			mSceneBulkSelectManager.stopSelect();
			GuiContext::getInstance().getSelectionManager().finishBulkSelection();
			update();
		}
	}
}

void SceneGlWidget::mouseMoveEvent(QMouseEvent * event ) {
	mSceneMouseInfo.x=event->pos().x();
	mSceneMouseInfo.y=event->pos().y();

	if (mViewportMoveInfo.isOnMove) {
		int distanceX=mViewportMoveInfo.startX-event->pos().x();
		int distanceY=mViewportMoveInfo.startY-event->pos().y();
		mCamera.move(-distanceX/mCamera.getScaleFactor(), -distanceY/mCamera.getScaleFactor(), 0.0);
		mViewportMoveInfo.startX=event->pos().x();
		mViewportMoveInfo.startY=event->pos().y();

		this->update();//update();
	} else if (GuiContext::getInstance().getCurrentTool()==Tool::Brush) {
		if (GuiContext::getInstance().getCurrentPaintCanvas() &&
			GuiContext::getInstance().getMainWindow().getBrushDock().getSelectedBrushNode()) {
			this->update();
		}
	} else if (GuiContext::getInstance().getCurrentTool()==Tool::Selection) {
		if (mSceneItemResizeManager.isOnResize()) {
			bool syncXY=!event->modifiers().testFlag(Qt::ControlModifier);
			bool requireUpdate=mSceneItemResizeManager.updateResize(syncXY, event->pos().x(), event->pos().y());
			if (requireUpdate) {
				this->update();
			}
		} else if (mSceneItemMoveManager.isOnMove()) {
			bool requireUpdate=mSceneItemMoveManager.updateMove(event->pos().x(), event->pos().y());
			if (requireUpdate) {
				this->update();
			}
		} else if (mSceneBulkSelectManager.isOnSelect()) {
			bool requireUpdate=mSceneBulkSelectManager.updateSelect(event->pos().x(), event->pos().y());
			if (requireUpdate) {
				vector<Node*> v;
				WorldCalculator::intersectTestForScene(v, GuiContext::getInstance().getCurrentScene(),
													   mSceneBulkSelectManager.getStartWorldX(),
													   mSceneBulkSelectManager.getStartWorldY(),
													   mSceneBulkSelectManager.getCurrentWorldX()-mSceneBulkSelectManager.getStartWorldX(),
													   mSceneBulkSelectManager.getCurrentWorldY()-mSceneBulkSelectManager.getStartWorldY(),
													   false);
				GuiContext::getInstance().getSelectionManager().deselectAllNodes();
				for (Node* n: v) {
					GuiContext::getInstance().getSelectionManager().setNodeAsSelectedInBulk(n);
				}
				this->update();
			}
		}
	}
}

#if 0
void SceneGlWidget::hoverMove(QHoverEvent *event) {
	if(GuiContext::getInstance().getSelectionManager().haveSelectedNodes()) {
	   GLMVector3 worldCoord=mCamera.unproject(event->pos().x(), event->pos().y());
	   const vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
	   bool isOverSelection=false;
	   for (Node* n : v) {
		   if (WorldCalculator::isNodeIntersecting(n, worldCoord.getX(), worldCoord.getY())) {
			   isOverSelection=true;
			   break;
		   }
	   }
	   GuiContext::getInstance().getSelectionManager().setIsMouseOverSelection(isOverSelection);
	}
}
#endif
bool SceneGlWidget::event(QEvent *e) {
	bool handled=false;
	switch(e->type()) {
	case QEvent::Enter:
		this->setMouseTracking(true);
		mSceneMouseInfo.isInside=true;
		break;
	case QEvent::Leave:
		this->setMouseTracking(false);
		mSceneMouseInfo.isInside=false;
		break;;
	case QEvent::HoverMove:
		//hoverMove(static_cast<QHoverEvent*>(e));
		break;
	default:
		break;
	}
	//cout << e->type()<< endl;

	if (!handled) {
		return QWidget::event(e);
	}
	return handled;
}

void SceneGlWidget::onLeftMouseClicked(QMouseEvent *event, int mx, int my) {
	//mSceneBulkSelectManager.stopSelect();

	if (GuiContext::getInstance().getCurrentTool()==Tool::Selection) {
		float x=(float)mx;
		float y=(float)my;
		std::cout << "View Matrix in Camera:\n" << mCamera.getViewMatrix().toString() << std::endl;
		//std::cout << "Proj Matrix:\n" << pr.toString() << std::endl;

		GLMVector3 pos=mCamera.unproject(mx, my);
		//glm::vec3 posVec=glm::make_vec3(pos.getPointer());

		cout<<"Mouse X=" << std::to_string(x)<<" Y="<< std::to_string(y)<<std::endl;
		cout<<"World X=" << std::to_string(pos.getX())<<" Y="<< std::to_string(pos.getY())<<std::endl;
		//cout<<"World X=" << std::to_string(posVec.x*this->size().width()/2.0)<<" Y="<< std::to_string(this->size().height()/2.0*posVec.y)<<std::endl;

		if (GuiContext::getInstance().getSelectionManager().haveSelectedNodes()) {
			const vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
			for (Node* n : v) {
				if (n->getNodeType()==NodeType::Sprite) {
					NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);
					bool intersecting=false;
					for (int i=0;i<3;i++) {
						PointFloat *rResizeHandle=nullptr;
						bool rXonly=false;
						bool rYonly=false;
						if (i==0) {
							//Bottom Right
							rResizeHandle=rNodeSprite->getResizeHandleBottomRight();
						} else if (i==1) {
							// Right, resize to X only
							rResizeHandle=rNodeSprite->getResizeHandleRight();
							rXonly=true;
						} else if (i==2) {
							// Bottom, resize to Y only
							rResizeHandle=rNodeSprite->getResizeHandleBottom();
							rYonly=true;
						}
						if (rResizeHandle) {
							if (WorldCalculator::isBoxIntersecting(rResizeHandle, pos.getX(), pos.getY(), 1.0, 1.0)) {
								mSceneItemResizeManager.startResize(rResizeHandle, rXonly, rYonly, &mCamera, rNodeSprite, mx, my);
								intersecting=true;
								break;
							}
						}
					}
					if (intersecting) {
						break;
					}
				}
			}
		}
		if (!mSceneItemResizeManager.isOnResize()) {
			vector<Node*> v;
			WorldCalculator::intersectTestForScene(v, GuiContext::getInstance().getCurrentScene(), pos.getX(), pos.getY(), 1.0, 1.0, true);
			bool startMoveOnExistingSelection=false;
			if (!event->modifiers().testFlag(Qt::ControlModifier)) {
				for (Node* n : v) {
					if (GuiContext::getInstance().getSelectionManager().isNodeSelected(n)) {
						startMoveOnExistingSelection=true;
					}
				}
				if (!startMoveOnExistingSelection) {
					GuiContext::getInstance().getSelectionManager().deselectAllNodes();
				}
			}
			if (!startMoveOnExistingSelection) {
				for (Node* n : v) {
					bool select=false;
					bool deselect=false;
					if (event->modifiers().testFlag(Qt::ControlModifier)) {
						if (GuiContext::getInstance().getSelectionManager().isNodeSelected(n)) {
							deselect=true;
						} else {
							select=true;
						}
					} else {
						select=true;
					}
					if (select) {
						GuiContext::getInstance().setSceneNodeInTreeAsSelected(n);
						GuiContext::getInstance().getSelectionManager().setNodeAsSelected(n);
					} else if (deselect) {
						//GuiContext::getInstance().setSceneNodeInTreeAsSelected(n);
						GuiContext::getInstance().getSelectionManager().deselectNode(n);
					}
				}
			}
			if (GuiContext::getInstance().getSelectionManager().haveSelectedNodes()) {
				mSceneItemMoveManager.startMove(&mCamera, mx, my);
			} else {
				mSceneBulkSelectManager.startSelect(&mCamera, mx, my);
				GuiContext::getInstance().getSelectionManager().startBulkSelection();
			}
		}
		update();
	} else if ( GuiContext::getInstance().getCurrentTool()==Tool::Brush &&
				GuiContext::getInstance().getCurrentPaintCanvas() &&
				GuiContext::getInstance().getMainWindow().getBrushDock().getSelectedBrushNode()) {
		if (GuiContext::getInstance().isNodeVisibleOn(GuiContext::getInstance().getCurrentPaintCanvas())
			&& !GuiContext::getInstance().isNodeLocked(GuiContext::getInstance().getCurrentPaintCanvas())
				) {
			GLMVector3 pos=mCamera.unproject(mx, my);
			vector<NodeSprite*> v=GuiContext::getInstance().getMainWindow().getBrushDock().getNewNodesFromBrush(pos.getX(), pos.getY());
			if (!v.empty()) {
				for (NodeSprite *rNodeSprite : v) {
					GuiContext::getInstance().insertNewSceneNode(rNodeSprite);
				}
				update();
			}
		}
	}
}


void SceneGlWidget::resizeGL(int w, int h)
{
	mCamera.setViewport(w,h);
	if (!mCameraInitialized) {
		mCameraInitialized=true;
		//mCamera.move(-float(w)/2.0, -float(h)/2.0,0.0);
	}
    updateViewport();
#if 0
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,5,0,0,0,0,1,0);
#endif
}



void SceneGlWidget::paintGL()
{
	// Draw the scene:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLoadMatrixf (mCamera.getViewMatrix().getPointer());

	glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(-5,-5,0);
		glVertex3d(5,-5,0);
		glVertex3d(5,5,0);
		glVertex3d(-5,5,0);
	glEnd();

	// draw Screen Frame
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3d(-mVirtualWidth/2.0,-mVirtualHeight/2.0,0);
		glVertex3d(mVirtualWidth/2.0,-mVirtualHeight/2.0,0);
		glVertex3d(mVirtualWidth/2.0,mVirtualHeight/2.0,0);
		glVertex3d(-mVirtualWidth/2.0,mVirtualHeight/2.0,0);
	glEnd();

	//glPushMatrix();
	//glTranslatef(mVirtualWidth/2.0, mVirtualHeight/2.0,0);
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3d(-10,0,0);
		glVertex3d(10,0,0);
		glVertex3d(0,-10,0);
		glVertex3d(0,10,0);
	glEnd();
	//glPopMatrix();


	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	//resize(1280,768);


	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-50, -50, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f( 50, -50, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f( 0.0,  50, 0);
	glEnd();

	glColor3f(1.0,1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	NodeScene *scene=GuiContext::getInstance().getCurrentScene();
	if (scene) {
		GLMMatrix4 m;
        WorldCalculator::updateNodeMatrix(m, scene);
		paintNode(scene, false);
	}

	glDisable(GL_TEXTURE_2D);
	vector<Node*> v=GuiContext::getInstance().getSelectionManager().getSelectedNodes();
	for (Node *n : v) {
		if (n->getNodeType()==NodeType::Sprite) {
			NodeSprite* rNodeSprite=static_cast<NodeSprite*>(n);

			rNodeSprite->setResizeHandleSizeLocal(10, 10);


			glm::mat4 viewMatrix=glm::make_mat4(mCamera.getViewMatrix().getPointer());
			glm::mat4 modelMatrix=glm::make_mat4(rNodeSprite->getCurrentModelMatrix().getPointer());
			viewMatrix*=modelMatrix;
			glLoadMatrixf (glm::value_ptr(viewMatrix));

			//if (GuiContext::getInstance().getSelectionManager().isNodeSelected(rNode)) {
				//if (GuiContext::getInstance().getSelectionManager().isMouseOverSelection()) {
				//	glColor3f(0.1, 0.3, 0.0);
				//} else {
			if (GuiContext::getInstance().getSelectionManager().getLatestSelected()==n) {
				glColor3f(0.3, 0.5, 1.0);
			} else {
				glColor3f(0.3, 0.5, 0.0);
			}
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
			glLineWidth(0.5);
			float w2=rNodeSprite->getSize().x/2.0;
			float h2=rNodeSprite->getSize().y/2.0;
			glBegin(GL_LINE_STRIP);
				glVertex3d(-w2,-h2,0);
				glVertex3d( w2, -h2,0);
				glVertex3d( w2, h2,0);
				glVertex3d(-w2, h2,0);
				glVertex3d(-w2,-h2,0);
			glEnd();

			glColor3f(1.0, 0.5, 0.0);


			//glm::vec4 screenBottomRight=viewMatrix*glm::vec4(w2, h2, 0.0, 1.0);
			//GLMVector3 a=mCamera.unproject(screenBottomRight.x, screenBottomRight.y);
			//screenBottomRight.x+=10;
			//screenBottomRight.y+=10;
			//GLMVector3 b=mCamera.unproject(screenBottomRight.x, screenBottomRight.y);
			//float diffX=b.getX()-a.getX();
			//float diffY=b.getY()-a.getY();

			//rNodeSprite->setResizeHandleSizeLocal(diffX, diffY);

			for (int i=0;i<3;i++) {
				PointFloat *rResizeHandleLocal;
				if (i==0) {
					rResizeHandleLocal=rNodeSprite->getResizeHandleLocalBottomRight();
				} else if (i==1) {
					rResizeHandleLocal=rNodeSprite->getResizeHandleLocalRight();
				} else if (i==2) {
					rResizeHandleLocal=rNodeSprite->getResizeHandleLocalBottom();
				}
				glBegin(GL_LINE_STRIP);
					glVertex3d(rResizeHandleLocal[0].x, rResizeHandleLocal[0].y,0);
					glVertex3d(rResizeHandleLocal[1].x, rResizeHandleLocal[1].y,0);
					glVertex3d(rResizeHandleLocal[2].x, rResizeHandleLocal[2].y,0);
					glVertex3d(rResizeHandleLocal[3].x, rResizeHandleLocal[3].y,0);
					glVertex3d(rResizeHandleLocal[0].x, rResizeHandleLocal[0].y,0);
				glEnd();
			}
		}
	}
	if (mSceneBulkSelectManager.isOnSelect()) {
		glLoadMatrixf (mCamera.getViewMatrix().getPointer());

		glColor3f(0.3, 0.5, 1.0);
		glBegin(GL_LINE_STRIP);
			glVertex3d(mSceneBulkSelectManager.getStartWorldX(), mSceneBulkSelectManager.getStartWorldY(),0);
			glVertex3d(mSceneBulkSelectManager.getCurrentWorldX(), mSceneBulkSelectManager.getStartWorldY(),0);
			glVertex3d(mSceneBulkSelectManager.getCurrentWorldX(), mSceneBulkSelectManager.getCurrentWorldY(),0);
			glVertex3d(mSceneBulkSelectManager.getStartWorldX(), mSceneBulkSelectManager.getCurrentWorldY(),0);
			glVertex3d(mSceneBulkSelectManager.getStartWorldX(), mSceneBulkSelectManager.getStartWorldY(),0);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void SceneGlWidget::paintNode(Node* rNode, bool isBrushCanvas) {
	bool visible=true;
	if (rNode->getNodeType()==NodeType::Layer) {
		NodeLayer* rNodeLayer=static_cast<NodeLayer*>(rNode);
		visible=rNodeLayer->getVisible();
	}
	if (visible) {
		glPushMatrix();
		if (rNode->getNodeType()==NodeType::Layer || rNode->getNodeType()==NodeType::Sprite) {
			Node2d *paintNode2d=(Node2d*)rNode;
			int x=paintNode2d->getPosition().x;
			int y=paintNode2d->getPosition().y;
			int w=paintNode2d->getSize().x;
			float scaleX=paintNode2d->getScale().x;
			float scaleY=paintNode2d->getScale().y;
			float w2=w/2.0;
			int h=paintNode2d->getSize().y;
			float h2=h/2.0;
			int angle=paintNode2d->getRotation();
			if (angle>360.0) {
				angle=angle/360.0;
			} else if (angle<0.0) {
				angle=360.0+angle;
			}

			//glTranslatef(0, 0, 0);
			glTranslatef(x, y, 0.0);
			glRotatef(angle, 0.0, 0.0, 1.0);
			if (scaleX!=1.0 || scaleY!=1.0) {
				glScalef(scaleX, scaleY, 1.0);
			}
			//GLfloat matrix[16];
			//glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
			//setCurrentWordCoordinateToNode(paintNode2d, matrix);

			if (rNode->getNodeType()==NodeType::Sprite) {
				NodeSprite *paintNode=static_cast<NodeSprite*>(rNode);
				NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(paintNode->getFrameRef().resourcefile);
				if (!rNodeResource) {
					rNodeResource=ProjectContext::getInstance().getDefaultResource();
				}
				if (rNodeResource) {
					NodeTexture *rNodeTexture=nullptr;
					NodeTextureFrame *rNodeTextureFrame=nullptr;

					Node *rNodeRef=rNodeResource->getNodeWithNodeId(paintNode->getFrameRef().textureid);
					if (rNodeRef) {
						if (rNodeRef->getNodeType()==NodeType::Texture) {
							rNodeTexture=static_cast<NodeTexture*>(rNodeRef);
							if (!paintNode->getFrameRef().frame.empty()) {
								rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithName(paintNode->getFrameRef().frame));
							}
						} else if (rNodeRef->getNodeType()==NodeType::Animation) {
							NodeAnimation* rNodeAnimation=static_cast<NodeAnimation*>(rNodeRef);
							if (!paintNode->getFrameRef().frame.empty()) {
								NodeAnimationFrame* rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNodeAnimation->getChildNodeWithNameAndNodeType(paintNode->getFrameRef().frame, NodeType::AnimationFrame));
								if (rNodeAnimationFrame) {
									NodeResource* rNodeResourceAnimationFrame=ProjectContext::getInstance().getOrLoadResourceByName(rNodeAnimationFrame->getFrameRef().resourcefile);
									if (rNodeResourceAnimationFrame) {
										rNodeTexture=static_cast<NodeTexture*>(rNodeResourceAnimationFrame->getNodeWithNodeId(rNodeAnimationFrame->getFrameRef().textureid));
										if (rNodeTexture && !rNodeAnimationFrame->getFrameRef().frame.empty()) {
											rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithName(rNodeAnimationFrame->getFrameRef().frame));
										}
									}
								}
							}
						}
					}
					if (rNodeTexture) {
						BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
						if (bTexture) {
							float tx=0.0;
							float ty=0.0;
							float tw=1.0;
							float th=1.0;

							if (rNodeTextureFrame) {
								if (rNodeTextureFrame->getFrame().width>0 && rNodeTextureFrame->getFrame().height>0) {
									if (rNodeTextureFrame->getFrame().x>0) {
										tx=((float)rNodeTextureFrame->getFrame().x)/((float)bTexture->width);
									}
									if (rNodeTextureFrame->getFrame().y>0) {
										ty=((float)rNodeTextureFrame->getFrame().y)/((float)bTexture->height);
									}
									tw=(((float)rNodeTextureFrame->getFrame().x)+((float)rNodeTextureFrame->getFrame().width))/((float)bTexture->width);
									th=(((float)rNodeTextureFrame->getFrame().y)+((float)rNodeTextureFrame->getFrame().height))/((float)bTexture->height);
								}
							}
							if (paintNode->getFlipX()) {
								float tmp=tx;
								tx=tw;
								tw=tmp;
							}
							if (paintNode->getFlipY()) {
								float tmp=ty;
								ty=th;
								th=tmp;
							}

							glEnable(GL_TEXTURE_2D);
							glPolygonMode(GL_FRONT, GL_FILL);
							glPolygonMode(GL_BACK, GL_FILL);
							glColor3f(1.0,1.0, 1.0);
							bTexture->bind();

							glBegin(GL_QUADS);
								//glTexCoord2f(0, 0); glVertex3f(-w2, -h2, 0);
								//glTexCoord2f(0, 1); glVertex3f(-w2,  h2, 0);
								//glTexCoord2f(1, 1); glVertex3f( w2,  h2, 0);
								//glTexCoord2f(1, 0); glVertex3f( w2, -h2, 0);
								glTexCoord2f(tx, ty); glVertex3f(-w2, -h2, 0);
								glTexCoord2f(tx, th); glVertex3f(-w2,  h2, 0);
								glTexCoord2f(tw, th); glVertex3f( w2,  h2, 0);
								glTexCoord2f(tw, ty); glVertex3f( w2, -h2, 0);
							glEnd();
						}
					}
				}
			}
		}
		unsigned long count=rNode->getChildCount();
		unsigned long i;
		for (i=0;i<count;i++) {
			Node *child=rNode->getNodeFromIndex(i);
			paintNode(child, isBrushCanvas);
		}
		if (
			mSceneMouseInfo.isInside &&
			rNode==GuiContext::getInstance().getCurrentPaintCanvas() &&
			GuiContext::getInstance().getCurrentTool()==Tool::Brush &&
			GuiContext::getInstance().getMainWindow().getBrushDock().getSelectedBrushNode()) {


			GLMVector3 v=mCamera.unproject(mSceneMouseInfo.x, mSceneMouseInfo.y);
			vector<NodeSprite*> &vv=GuiContext::getInstance().getMainWindow().getBrushDock().getNodesFromBrush(v.getX(), v.getY());
			for (NodeSprite* rNodeBrush : vv) {
				paintNode(rNodeBrush, true);
			}
		}
		glPopMatrix();
	}
}
