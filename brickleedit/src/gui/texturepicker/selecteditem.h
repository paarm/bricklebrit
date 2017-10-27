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
	int			offsetX;
	int			offsetY;
	PointInt    sizeWH;
    PointFloat  scale;
    float       rotation;
    bool        flipX;
    bool        flipY;
};

struct BrushInfoItem {
	SelectedItem		rSelectedItem;
	SelectedItemPref	rSelectedItemPref;
};

#endif // SELECTEDITEM_H
