#include "texturemanager.h"
#include "../utils/dirutil.h"

TextureManager::TextureManager() {
}


TextureManager::~TextureManager() {
	clear();
}

void TextureManager::setBasePath(const string& rBasePath) {
	mBasePath=rBasePath;
}

void TextureManager::clear() {
	for(auto t : mBTexturePngMap) {
		delete t.second;
	}
	mBTexturePngMap.clear();
}

BTexturePng* TextureManager::getTexture(const string &rPathRelativeToBasePath) {
	BTexturePng* rv=nullptr;
	auto element=mBTexturePngMap.find(rPathRelativeToBasePath);
	if (element!=mBTexturePngMap.end()) {
		rv=element->second;
	}
	if (!rv) {
		string path=DirUtil::concatPath(mBasePath, rPathRelativeToBasePath);
		BTexturePng *rBTexturePng=new BTexturePng();
		rBTexturePng->load(path);
		mBTexturePngMap[rPathRelativeToBasePath]=rBTexturePng;
		rv=rBTexturePng;
	}
	return rv;
}

