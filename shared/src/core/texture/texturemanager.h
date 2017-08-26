#pragma once
#include <map>
#include <string>
#include "btexturepng.h"

using namespace std;

class TextureManager {
private:
	string						mBasePath="";
	map<string, BTexturePng*>	mBTexturePngMap;
public:
	TextureManager();
	void setBasePath(const string& rBasePath);
	void clear();
	~TextureManager();
	BTexturePng* getTexture(const string &rPathRelativeToBasePath);
};
