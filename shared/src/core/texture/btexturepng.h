#pragma once

#include <vector>
#include <string>
#include "../thirdparty/lodepng/lodepng.h"
#include <GL/glew.h>
//#include <GL/glcorearb.h>
//#include <GL/glext.h>
//#include <GL/glext.h>
#include <iostream>

using namespace std;

class BTexturePng
{
private:
	bool	loaded=false;
	std::vector<unsigned char> image;
    GLuint glTextureId=0;
public:
	unsigned width=0, height=0;

	bool load(const string &rFileName) {
		if (loaded) {
			image.clear();
			image.shrink_to_fit();
			loaded=false;
		}
		unsigned error = lodepng::decode(image, width, height, rFileName);
		if (error==0) {
			loaded=true;
		} else {
			cout << "Could not load PNG "<< rFileName <<'\n';
		}
		return loaded;
	}

	unsigned char* getRawData() {
		unsigned char*rv=nullptr;
		if (loaded) {
			rv=&image[0];
		}
		return rv;
	}

	unsigned int getRawDataLength() {
		unsigned int rv=0;
		if (loaded) {
			rv=image.size();
		}
		return rv;
	}

	void bind() {
        if (loaded) {
			if (glTextureId==0) {
				glEnable(GL_TEXTURE_2D);
				glGenTextures(1, &glTextureId);
				glBindTexture(GL_TEXTURE_2D, glTextureId);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
				//glBindTexture(GL_TEXTURE_2D, glTextureId);
			} else {
				GLint glTextureIdCurrent=0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &glTextureIdCurrent);
				if (glTextureIdCurrent!=static_cast<int>(glTextureId)) {
					glBindTexture(GL_TEXTURE_2D, glTextureId);
				}
			}
        }
	}
};

