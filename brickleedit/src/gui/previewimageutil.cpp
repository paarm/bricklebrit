#include "previewimageutil.h"
#include "../project/projectcontext.h"


PreviewImageUtil::PreviewImageUtil()
{

}

QPixmap PreviewImageUtil::getPreviewImage(SelectedItem &rSelectedItem, int x, int y) {
	if (rSelectedItem.rNodeTexture) {
		BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rSelectedItem.rNodeTexture->getPath());
		if (bTexture) {
			QImage rImageSrc(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
			if (rSelectedItem.rNodeTextureFrame) {
				QImage rImageFrame=rImageSrc.copy(rSelectedItem.rNodeTextureFrame->getFrame().x, rSelectedItem.rNodeTextureFrame->getFrame().y, rSelectedItem.rNodeTextureFrame->getFrame().width, rSelectedItem.rNodeTextureFrame->getFrame().height);
				rImageFrame=rImageFrame.scaled(QSize(x,y), Qt::KeepAspectRatio);
				return QPixmap::fromImage(rImageFrame);
			} else {
				QImage rImageFrame=rImageSrc.scaled(QSize(x,y), Qt::KeepAspectRatio);
				return QPixmap::fromImage(rImageFrame);
			}
		}
	}
	return QPixmap();
}
