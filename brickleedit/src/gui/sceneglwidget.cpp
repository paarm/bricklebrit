#include "sceneglwidget.h"
#include "guicontext.h"

void SceneGlWidget::paintGL()
{
	// Draw the scene:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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
		glVertex3d(0,0,0);
		glVertex3d(mWidth,0,0);
		glVertex3d(mWidth,mHeight,0);
		glVertex3d(0,mHeight,0);
	glEnd();

	glPushMatrix();
	glTranslatef(mWidth/2.0, mHeight/2.0,0);
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3d(-10,0,0);
		glVertex3d(10,0,0);
		glVertex3d(0,-10,0);
		glVertex3d(0,10,0);
	glEnd();
	glPopMatrix();


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
#if 0
	texture->bind();
	// Draw a textured quad

	glPushMatrix();
	glTranslatef(-200, 0,0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 100, 0);
	glTexCoord2f(1, 1); glVertex3f(100, 100, 0);
	glTexCoord2f(1, 0); glVertex3f(100, 0, 0);
	glEnd();
	glPopMatrix();
	texture->release();
	glPushMatrix();
	glTranslatef(200, 0,0);
	//glBindTexture(GL_TEXTURE_2D, texName);
	bTexture.bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
		glTexCoord2f(0, 1); glVertex3f(0, 100, 0);
		glTexCoord2f(1, 1); glVertex3f(100, 100, 0);
		glTexCoord2f(1, 0); glVertex3f(100, 0, 0);
	glEnd();
	glPopMatrix();
	//glBindTexture(GL_TEXTURE_2D, texName);
#endif



	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	NodeScene *scene=ProjectContext::getInstance().getNodeScene();
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
		float w2=w/2.0;
		int h=paintNode->getSize().y;
		float h2=h/2.0;
		int tx=paintNode->getTextureSourceRect().x;
		int ty=paintNode->getTextureSourceRect().y;
		int tw=paintNode->getTextureSourceRect().width;
		int th=paintNode->getTextureSourceRect().height;
		int angle=paintNode->getRotation();
		if (angle>360.0) {
			angle=angle/360.0;
		} else if (angle<0.0) {
			angle=360.0+angle;
		}


		Node *rNodeT=ProjectContext::getInstance().getCurrentResource()->getNodeWithNodeId(paintNode->getTextureRef().refid);
		if (rNodeT && rNodeT->getNodeType()==NodeType::Texture) {
			NodeTexture *rNodeTexture=(NodeTexture*)rNodeT;
			BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
			if (bTexture) {
				//glTranslatef(0, 0, 0);
				glTranslatef(x, y, 0.0);
				glRotatef(angle, 0.0, 0.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				glPolygonMode(GL_FRONT, GL_FILL);
				glPolygonMode(GL_BACK, GL_FILL);
				glColor3f(1.0,1.0, 1.0);
				bTexture->bind();

				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(-w2, -h2, 0);
					glTexCoord2f(0, 1); glVertex3f(-w2,  h2, 0);
					glTexCoord2f(1, 1); glVertex3f( w2,  h2, 0);
					glTexCoord2f(1, 0); glVertex3f( w2, -h2, 0);
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
#if 0
		//glBindTexture(GL_TEXTURE_2D, texName);
		bTexture.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
			glTexCoord2f(0, 1); glVertex3f(0, 100, 0);
			glTexCoord2f(1, 1); glVertex3f(100, 100, 0);
			glTexCoord2f(1, 0); glVertex3f(100, 0, 0);
		glEnd();
#endif
	}
	unsigned long count=rNode->getChildCount();
	unsigned long i;
	for (i=0;i<count;i++) {
		Node *child=rNode->getNodeFromIndex(i);
		paintNode(child);
	}
	glPopMatrix();
}
