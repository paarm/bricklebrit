#ifndef CAMERA_H
#define CAMERA_H

#include "../math/glmwrapper.h"

class Camera {
private:
	int	viewportWidth;
	int viewportHeight;
	float x=0;
	float y=0;
	float z=0;

	const float minZ=-80;
	const float maxScaleMinusFactor=-0.999;
	const float maxZ=100;
	const float maxScalePlusFactor=8.0;
	float scaleFactor=1.0;
	GLMMatrix4 viewMatrixScale;
	GLMMatrix4 viewMatrixTranslate;
	GLMMatrix4 viewMatrix;
	bool dirty=true;
	void updateViewMatrix();
public:
	Camera();
	int getViewportWidth();
	int getViewportHeight();
	void setViewport(float width, float height);
	void move(float distX, float distY, float distZ);
	void moveTo(float posX, float posY, float posZ);
	float getScaleFactor();
	GLMVector3 getPosition();
	float getX();
	float getY();
	float getZ();
	GLMMatrix4 getViewMatrix();
	GLMMatrix4 getViewMatrixScale();
	GLMMatrix4 getViewMatrixTranslate();
	GLMVector3 unproject(float mouseX, float mouseY, GLMMatrix4 projectionMatrix);

};

#endif // CAMERA_H
