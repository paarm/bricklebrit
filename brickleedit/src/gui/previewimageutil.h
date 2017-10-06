#ifndef PREVIEWIMAGEUTIL_H
#define PREVIEWIMAGEUTIL_H
#include <QPixmap>
#include "texturepicker/selecteditem.h"
class PreviewImageUtil
{
private:
	PreviewImageUtil();
public:
	static QPixmap getPreviewImage(SelectedItem &rSelectedItem, int x, int y);
};

#endif // PREVIEWIMAGEUTIL_H
