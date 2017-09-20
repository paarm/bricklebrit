#ifndef GLMVECTOR3_H
#define GLMVECTOR3_H
#include <memory.h>
#include <iostream>
#include <string>

struct GLMVector3 {
	float v3[3]={0};
	GLMVector3(float x, float y, float z) {
		v3[0]=x;
		v3[1]=y;
		v3[2]=z;
	}

	float* getPointer() {
		return v3;
	}
	void setFromPointer(float* v3_src) {
		memcpy(v3,v3_src, sizeof(float)*3);
	}
	std::string toString() {
		std::string rv=std::to_string(v3[0])+","+std::to_string(v3[1])+","+std::to_string(v3[2]);
		return rv;
	}
};

struct GLMMatrix4 {
	GLMMatrix4() {
		toIdentidy();
	}
	GLMMatrix4(float* m4_src) {
		setFromPointer(m4_src);
	}

	void toIdentidy() {
		m4[0]=1.0;
		m4[5]=1.0;
		m4[10]=1.0;
		m4[15]=1.0;
	}

	float m4[16]={0.0};
	float* getPointer() {
		return &m4[0];
	}
	void setFromPointer(float* m4_src) {
		if (m4_src) {
			memcpy(&m4[0],m4_src, sizeof(float[16]));
		} else {
			toIdentidy();
		}
	}
	std::string toString() {
		std::string rv=std::to_string(m4[0])+","+std::to_string(m4[1])+","+std::to_string(m4[2])+","+std::to_string(m4[3])+"+\n"+
				std::to_string(m4[4])+","+std::to_string(m4[5])+","+std::to_string(m4[6])+","+std::to_string(m4[7])+"+\n"+
				std::to_string(m4[8])+","+std::to_string(m4[9])+","+std::to_string(m4[10])+","+std::to_string(m4[11])+"+\n"+
				std::to_string(m4[12])+","+std::to_string(m4[13])+","+std::to_string(m4[14])+","+std::to_string(m4[15]);
		return rv;
	}
};

#endif // GLMVECTOR3_H
