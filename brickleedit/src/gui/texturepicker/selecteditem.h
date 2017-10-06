#ifndef SELECTEDITEM_H
#define SELECTEDITEM_H
#include <bricklelib.h>

struct SelectedItem {
	string resourceName;
	NodeTexture			*rNodeTexture=nullptr;
	NodeTextureFrame	*rNodeTextureFrame=nullptr;
	NodeAnimation		*rNodeAnimation=nullptr;
	NodeAnimationFrame	*rNodeAnimationFrame=nullptr;
};

#endif // SELECTEDITEM_H
