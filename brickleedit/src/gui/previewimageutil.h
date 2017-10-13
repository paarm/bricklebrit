#ifndef PREVIEWIMAGEUTIL_H
#define PREVIEWIMAGEUTIL_H
#include <QPixmap>
#include <bricklelib.h>
class PreviewImageUtil
{
private:
	PreviewImageUtil();
public:
	static QPixmap getPreviewImage(NodeTexture* rNodeTexture, NodeTextureFrame* rNodeTextureFra, int x, int y);
};

#endif // PREVIEWIMAGEUTIL_H
