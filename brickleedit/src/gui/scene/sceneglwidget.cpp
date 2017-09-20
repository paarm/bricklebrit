#include "sceneglwidget.h"
#include "../guicontext.h"
#include "../../project/projectcontext.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	glMatrixMode(GL_MODELVIEW);
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
		onMouseClicked(event->pos().x(), event->pos().y());
	}
}

void SceneGlWidget::mouseReleaseEvent(QMouseEvent * event ) {
	if (event->button()==Qt::MiddleButton) {
		mViewportMoveInfo.isOnMove=false;
		mViewportMoveInfo.startX=0;
		mViewportMoveInfo.startY=0;
	}
}

void SceneGlWidget::mouseMoveEvent(QMouseEvent * event ) {
	if (mViewportMoveInfo.isOnMove) {
		int distanceX=mViewportMoveInfo.startX-event->pos().x();
		int distanceY=mViewportMoveInfo.startY-event->pos().y();
		mCamera.move(-distanceX/mCamera.getScaleFactor(), -distanceY/mCamera.getScaleFactor(), 0.0);
		mViewportMoveInfo.startX=event->pos().x();
		mViewportMoveInfo.startY=event->pos().y();

		this->update();//update();
	}
}

void SceneGlWidget::intersectTestForNode(glm::mat4 rMatrix, Node* rNode, float worldX, float worldY) {
	if (rNode) {
		if (rNode->getNodeType()==NodeType::Sprite) {
			vector<PointFloat> v1;
			vector<PointFloat> v2;
			v2.emplace_back(worldX, worldY);
			v2.emplace_back(worldX+1.0, worldY+1.0);
			NodeSprite *rNodeSprite=static_cast<NodeSprite*>(rNode);
			rMatrix=getLocalMatrix(rMatrix, rNodeSprite->getPosition().x, rNodeSprite->getPosition().y, 0.0, rNodeSprite->getSize().x, rNodeSprite->getSize().y, rNodeSprite->getScale().x, rNodeSprite->getScale().y, rNodeSprite->getRotation());
			//rMatrix*=rLocalMatrix;
			setCurrentWordCoordinateToNode(rNodeSprite, rMatrix);

			v1.emplace_back(rNodeSprite->getCurrentLeftTop());
			v1.emplace_back(rNodeSprite->getCurrentRightTop());
			v1.emplace_back(rNodeSprite->getCurrentRightBottom());
			v1.emplace_back(rNodeSprite->getCurrentLeftBottom());
			v1.emplace_back(rNodeSprite->getCurrentLeftTop());
			if (Intersection::isPolygonsIntersecting(v1, v2)) {
				GuiContext::getInstance().setSceneNodeAsSelected(rNode);
			}
		}
		int childCount=rNode->getChildCount();
		for (int i=0;i<childCount;i++) {
			Node *rNodeChild=rNode->getNodeFromIndex(i);
			intersectTestForNode(rMatrix, rNodeChild, worldX, worldY);
		}
	}
}

void SceneGlWidget::onMouseClicked(int mx, int my) {
	float x=(float)mx;
	float y=(float)my;
	GLfloat matrixArray[16];
	glGetFloatv (GL_PROJECTION_MATRIX, matrixArray);
	glm::mat4x4 projectionMatrix=glm::make_mat4x4(matrixArray);
	GLMMatrix4 pr;
	pr.setFromPointer(matrixArray);
	std::cout << "View Matrix in Camera:\n" << mCamera.getViewMatrix().toString() << std::endl;
	std::cout << "Proj Matrix:\n" << pr.toString() << std::endl;

	GLMVector3 pos=mCamera.unproject(mx, my, pr);
	glm::vec3 posVec=glm::make_vec3(pos.getPointer());

	cout<<"Mouse X=" << std::to_string(x)<<" Y="<< std::to_string(y)<<std::endl;
	cout<<"World X Factor=" << std::to_string(posVec.x)<<" Y="<< std::to_string(posVec.y)<<std::endl;
	cout<<"World X=" << std::to_string(posVec.x*this->size().width()/2.0)<<" Y="<< std::to_string(this->size().height()/2.0*posVec.y)<<std::endl;

	GuiContext::getInstance().clearSelectedSceneNodes();
	intersectTestForNode(glm::mat4(1.0), GuiContext::getInstance().getCurrentScene(), posVec.x*this->size().width()/2.0, this->size().height()/2.0*posVec.y);
	update();
}

void SceneGlWidget::setCurrentWordCoordinateToNode(Node2d *rNode2d, glm::mat4x4 &matrix) {
	//int x=rNode2d->getPosition().x;
	//int y=rNode2d->getPosition().y;
	int w=rNode2d->getSize().x;
	float w2=w/2.0;
	int h=rNode2d->getSize().y;
	float h2=h/2.0;

	//glm::mat4x4 matrix=glm::make_mat4x4(matrixArray);
	glm::vec4 rCenter=matrix*glm::vec4{0.0,0.0,0.0,1.0};
	glm::vec4 rLT=matrix*glm::vec4{-w2,-h2,0.0,1.0};
	glm::vec4 rLB=matrix*glm::vec4{-w2,h2,0.0,1.0};
	glm::vec4 rRT=matrix*glm::vec4{w2,-h2,0.0,1.0};
	glm::vec4 rRB=matrix*glm::vec4{w2,h2,0.0,1.0};
	rNode2d->setCurrentPos(rCenter.x, rCenter.y, rLT.x, rLT.y, rLB.x, rLB.y, rRT.x, rRT.y, rRB.x, rRB.y);
	cout<<rNode2d->getName()<<std::endl;
	cout<<"CE X=" << std::to_string(rCenter.x)<<"Y="<< std::to_string(rCenter.y)<<std::endl;
	cout<<"LT X=" << std::to_string(rLT.x)<<"Y="<< std::to_string(rLT.y)<<std::endl;
	cout<<"LB X=" << std::to_string(rLB.x)<<"Y="<< std::to_string(rLB.y)<<std::endl;
	cout<<"RT X=" << std::to_string(rRT.x)<<"Y="<< std::to_string(rRT.y)<<std::endl;
	cout<<"RB X=" << std::to_string(rRB.x)<<"Y="<< std::to_string(rRB.y)<<std::endl;
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

glm::mat4x4 SceneGlWidget::getLocalMatrix(glm::mat4x4 m, float x, float y, float z, float w, float h, float scaleX, float scaleY, float rotation) {
	//glm::mat4 m(1.0);


	float angle=rotation;
	if (angle>360.0) {
		angle=angle/360.0;
	} else if (angle<0.0) {
		angle=360.0+angle;
	}

	m=glm::translate(m, glm::vec3(x,y,0.0));
	m=glm::rotate(m, angle, glm::vec3(0.0,0.0,1.0));
	if (scaleX!=1.0 || scaleY!=1.0) {
		m=glm::scale(m, glm::vec3(scaleX, scaleY, 1.0));
	}
	return m;
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
		paintNode(scene);
	}
	glDisable(GL_TEXTURE_2D);

}

void SceneGlWidget::paintNode(Node* rNode) {
	glPushMatrix();
	if (rNode->getNodeType()==NodeType::Sprite) {
		NodeSprite *paintNode=(NodeSprite*)rNode;
		int x=paintNode->getPosition().x;
		int y=paintNode->getPosition().y;
		int w=paintNode->getSize().x;
		float scaleX=paintNode->getScale().x;
		float scaleY=paintNode->getScale().y;
		float w2=w/2.0;
		int h=paintNode->getSize().y;
		float h2=h/2.0;
		int angle=paintNode->getRotation();
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
		//setCurrentWordCoordinateToNode(paintNode, matrix);


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
					glDisable(GL_TEXTURE_2D);


					if (GuiContext::getInstance().isSceneNodeSelected(rNode)) {
						glPolygonMode(GL_FRONT, GL_LINE);
						glPolygonMode(GL_BACK, GL_LINE);
						glLineWidth(0.5);
						glColor3f(0.3, 0.5, 0.0);
						glBegin(GL_LINE_STRIP);
							glVertex3d(-w2,-h2,0);
							glVertex3d( w2, -h2,0);
							glVertex3d( w2, h2,0);
							glVertex3d(-w2, h2,0);
							glVertex3d(-w2,-h2,0);
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
		paintNode(child);
	}
	glPopMatrix();
}
