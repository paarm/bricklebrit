#include "previewimageutil.h"
#include "../project/projectcontext.h"


PreviewImageUtil::PreviewImageUtil()
{

}

QPixmap PreviewImageUtil::getPreviewImage(NodeTexture* rNodeTexture, NodeTextureFrame* rNodeTextureFrame, int x, int y) {
	if (rNodeTexture) {
		BTexturePng *bTexture=ProjectContext::getInstance().getTexture(rNodeTexture->getPath());
		if (bTexture) {
			QImage rImageSrc(bTexture->getRawData(), bTexture->width, bTexture->height, QImage::Format_RGBA8888);
			if (rNodeTextureFrame) {
				QImage rImageFrame=rImageSrc.copy(rNodeTextureFrame->getFrame().x, rNodeTextureFrame->getFrame().y, rNodeTextureFrame->getFrame().width, rNodeTextureFrame->getFrame().height);
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
