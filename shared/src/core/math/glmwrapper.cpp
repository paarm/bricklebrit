#include "glmwrapper.h"

GLMVector3::GLMVector3(float x, float y, float z) {
	v3[0]=x;
	v3[1]=y;
	v3[2]=z;
}
GLMVector3::GLMVector3(const GLMVector3& other) {
	v3[0]=other.v3[0];
	v3[1]=other.v3[1];
	v3[2]=other.v3[2];
}

float* GLMVector3::getPointer() {
	return v3;
}
void GLMVector3::setFromPointer(float* v3_src) {
	memcpy(v3,v3_src, sizeof(float)*3);
}
std::string GLMVector3::toString() {
	std::string rv=std::to_string(v3[0])+","+std::to_string(v3[1])+","+std::to_string(v3[2]);
	return rv;
}


GLMMatrix4::GLMMatrix4() {
	toIdentidy();
}
GLMMatrix4::GLMMatrix4(float* m4_src) {
	setFromPointer(m4_src);
}
GLMMatrix4::GLMMatrix4(GLMMatrix4& other) {
	setFromPointer(other.getPointer());
}
void GLMMatrix4::toIdentidy() {
	m4[0]=1.0;
	m4[5]=1.0;
	m4[10]=1.0;
	m4[15]=1.0;
}
float* GLMMatrix4::getPointer() {
	return &m4[0];
}
void GLMMatrix4::setFromPointer(float* m4_src) {
	if (m4_src) {
		memcpy(&m4[0],m4_src, sizeof(float[16]));
	} else {
		toIdentidy();
	}
}
std::string GLMMatrix4::toString() {
	std::string rv=std::to_string(m4[0])+","+std::to_string(m4[1])+","+std::to_string(m4[2])+","+std::to_string(m4[3])+"+\n"+
			std::to_string(m4[4])+","+std::to_string(m4[5])+","+std::to_string(m4[6])+","+std::to_string(m4[7])+"+\n"+
			std::to_string(m4[8])+","+std::to_string(m4[9])+","+std::to_string(m4[10])+","+std::to_string(m4[11])+"+\n"+
			std::to_string(m4[12])+","+std::to_string(m4[13])+","+std::to_string(m4[14])+","+std::to_string(m4[15]);
	return rv;
}
