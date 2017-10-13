#ifndef SELECTEDITEM_H
#define SELECTEDITEM_H
#include <bricklelib.h>

struct SelectedItem {
	string resourceName;
	NodeTexture			*rNodeTexture=nullptr;
	NodeTextureFrame	*rNodeTextureFrame=nullptr;
	NodeAnimation		*rNodeAnimation=nullptr;
	NodeAnimationFrame	*rNodeAnimationFrame=nullptr;
	void clear() {
		resourceName="";
		rNodeTexture=nullptr;
		rNodeTextureFrame=nullptr;
		rNodeAnimation=nullptr;
		rNodeAnimationFrame=nullptr;
	}
};

struct SelectedItemPref {
    PointInt    sizeWH;
    PointFloat  scale;
    float       rotation;
    bool        flipX;
    bool        flipY;
};

#endif // SELECTEDITEM_H
