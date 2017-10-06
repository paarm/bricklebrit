#ifndef GLMVECTOR3_H
#define GLMVECTOR3_H
#include <memory.h>
#include <iostream>
#include <string>

class GLMVector3 {
private:
	float v3[3]={0};
public:
	GLMVector3(float x, float y, float z);
	GLMVector3(const GLMVector3& other);
	float* getPointer();
	void setFromPointer(float* v3_src);
	std::string toString();
	float getX();
	float getY();
	float getZ();
};

class GLMMatrix4 {
private:
	float m4[16]={0.0};
public:
	GLMMatrix4();
	GLMMatrix4(float* m4_src);
	GLMMatrix4(GLMMatrix4& other);
	void toIdentidy();
	float* getPointer();
	void setFromPointer(float* m4_src);
	std::string toString();
};

#endif // GLMVECTOR3_H
