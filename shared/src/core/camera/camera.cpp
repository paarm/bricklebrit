#include "camera.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>

Camera::Camera() {
	updateViewMatrix();
}

void Camera::setViewport(float width, float height) {
	viewportWidth=width;
	viewportHeight=height;
}

void Camera::move(float distX, float distY, float distZ) {
	if (distX!=0.0 || distY!=0.0 || distZ!=0.0) {
		x+=distX;
		y+=distY;
		z+=distZ;
		if (z>maxZ) z=maxZ;
		if (z<minZ) z=minZ;
		updateViewMatrix();
	}
}

void Camera::moveTo(float posX, float posY, float posZ) {
	x=posX;
	y=posY;
	z=posZ;
	if (z>maxZ) z=maxZ;
	if (z<minZ) z=minZ;
	updateViewMatrix();
}

float Camera::getScaleFactor() {
	return scaleFactor;
}
float Camera::getX() {
	return x;
}
float Camera::getY() {
	return y;
}
float Camera::getZ() {
	return z;
}

int Camera::getViewportWidth() {
	return viewportWidth;
}

int Camera::getViewportHeight() {
	return viewportHeight;
}

GLMVector3 Camera::getPosition() {
	return GLMVector3(x,y,z);
}

void Camera::updateViewMatrix() {
	glm::mat4 tmpViewMatrixS=glm::mat4(1.0);
	glm::mat4 tmpViewMatrixT=glm::mat4(1.0);
	glm::mat4 tmpViewMatrix=glm::mat4(1.0);

	float sf=0.0;
	if (z!=0.0) {
		if (z>0.0) {
			sf=maxScalePlusFactor*(z/maxZ);
		} else {
			sf=maxScaleMinusFactor*(z/minZ);
			//if (sf<-1.0) sf=-1.0;
		}
	}
	scaleFactor=1.0+sf;
	tmpViewMatrixS=glm::scale(tmpViewMatrix, glm::vec3(scaleFactor,scaleFactor,1.0));
	viewMatrixScale.setFromPointer(glm::value_ptr(tmpViewMatrixS));
	tmpViewMatrixT=glm::translate(tmpViewMatrix, glm::vec3(x,y,0.0));
	viewMatrixTranslate.setFromPointer(glm::value_ptr(tmpViewMatrixT));
	tmpViewMatrix=tmpViewMatrixS*tmpViewMatrixT;
	viewMatrix.setFromPointer(glm::value_ptr(tmpViewMatrix));
	std::cout << "View Matrix changed to:\n" << viewMatrix.toString() << std::endl;
}

GLMMatrix4 Camera::getViewMatrix() {
	return viewMatrix;
}

GLMMatrix4 Camera::getViewMatrixScale() {
	return viewMatrixScale;
}
GLMMatrix4 Camera::getViewMatrixTranslate() {
	return viewMatrixTranslate;
}

GLMVector3 Camera::unproject(float mouseX, float mouseY, GLMMatrix4 projectionMatrix) {
	float _x = ((2.0/viewportWidth)*mouseX)-1.0f;
	float _y = ((2.0/viewportHeight)*mouseY)-1.0f;
	float _z = 0.0f;
	//std::cout << "Mouse click normalized X="<< std::to_string(_x) << " Y=" << std::to_string(_y) << std::endl;
	glm::vec4 ray_wor(_x,_y, _z, 1.0);
	ray_wor=glm::inverse(glm::make_mat4x4(projectionMatrix.getPointer()))*ray_wor;
	ray_wor=glm::inverse(glm::make_mat4x4(getViewMatrixScale().getPointer()))*ray_wor;
	glm::mat4x4 t=glm::inverse(glm::make_mat4x4(getViewMatrixTranslate().getPointer()));
	// normalize the translation part
	float *p=glm::value_ptr(t);
	p[12]=p[12]/(viewportWidth/2.0);
	p[13]=p[13]/(viewportHeight/2.0);
	ray_wor=t*ray_wor;
	GLMVector3 rv(ray_wor.x, ray_wor.y, ray_wor.z);
	std::cout << "unprojected="<< rv.toString() << std::endl;
	return rv;
}
