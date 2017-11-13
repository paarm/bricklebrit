#include "textureutil.h"
#include "../project/projectcontext.h"

void TextureUtil::getTexture(Node2d *paintNode, NodeTexture **rOutNodeTexture, NodeTextureFrame **rOutNodeTextureFrame) {
	if (paintNode) {
		NodeResource* rNodeResource=ProjectContext::getInstance().getOrLoadResourceByName(paintNode->getFrameRef().resourcefile);
		if (!rNodeResource) {
			rNodeResource=ProjectContext::getInstance().getDefaultResource();
		}
		if (rNodeResource) {
			NodeTexture *rNodeTexture=nullptr;
			NodeTextureFrame *rNodeTextureFrame=nullptr;

			Node *rNodeRef=rNodeResource->getNodeWithNodeId(paintNode->getFrameRef().textureid);
			if (rNodeRef) {
				if (rNodeRef->getNodeType()==NodeType::Texture) {
					rNodeTexture=static_cast<NodeTexture*>(rNodeRef);
					if (!paintNode->getFrameRef().frame.empty()) {
						rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getNodeFromIndex(atoi(paintNode->getFrameRef().frame.c_str())));
						//rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithName(paintNode->getFrameRef().frame));
					}
				} else if (rNodeRef->getNodeType()==NodeType::Animation) {
					NodeAnimation* rNodeAnimation=static_cast<NodeAnimation*>(rNodeRef);
					if (!paintNode->getFrameRef().frame.empty()) {
						NodeAnimationFrame* rNodeAnimationFrame=static_cast<NodeAnimationFrame*>(rNodeAnimation->getChildNodeWithNameAndNodeType(paintNode->getFrameRef().frame, NodeType::AnimationFrame));
						if (rNodeAnimationFrame) {
							NodeResource* rNodeResourceAnimationFrame=ProjectContext::getInstance().getOrLoadResourceByName(rNodeAnimationFrame->getFrameRef().resourcefile);
							if (rNodeResourceAnimationFrame) {
								rNodeTexture=static_cast<NodeTexture*>(rNodeResourceAnimationFrame->getNodeWithNodeId(rNodeAnimationFrame->getFrameRef().textureid));
								if (rNodeTexture && !rNodeAnimationFrame->getFrameRef().frame.empty()) {
									//rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getChildNodeWithName(rNodeAnimationFrame->getFrameRef().frame));
									rNodeTextureFrame=static_cast<NodeTextureFrame*>(rNodeTexture->getNodeFromIndex(atoi(rNodeAnimationFrame->getFrameRef().frame.c_str())));
								}
							}
						}
					}
				}
				(*rOutNodeTexture)=rNodeTexture;
				(*rOutNodeTextureFrame)=rNodeTextureFrame;
			}
		}
	}
}

void TextureUtil::getTextureCoords(Node2d* rNodeSprite, NodeTextureFrame *rNodeTextureFrame, BTexturePng *bTexture, float *outtx, float *outty, float *outtw, float* outth) {
	float tx=0.0;
	float ty=0.0;
	float tw=1.0;
	float th=1.0;

	if (rNodeTextureFrame) {
		if (rNodeTextureFrame->getFrame().width>0 && rNodeTextureFrame->getFrame().height>0) {
			if (rNodeTextureFrame->getFrame().x>0) {
				tx=((float)rNodeTextureFrame->getFrame().x)/((float)bTexture->width);
			}
			if (rNodeTextureFrame->getFrame().y>0) {
				ty=((float)rNodeTextureFrame->getFrame().y)/((float)bTexture->height);
			}
			tw=(((float)rNodeTextureFrame->getFrame().x)+((float)rNodeTextureFrame->getFrame().width))/((float)bTexture->width);
			th=(((float)rNodeTextureFrame->getFrame().y)+((float)rNodeTextureFrame->getFrame().height))/((float)bTexture->height);
		}
	}
#if 0
	if (rNodeSprite->getFlipX()) {
		float tmp=tx;
		tx=tw;
		tw=tmp;
	}
	if (rNodeSprite->getFlipY()) {
		float tmp=ty;
		ty=th;
		th=tmp;
	}
#endif
	(*outtx)=tx;
	(*outty)=ty;
	(*outtw)=tw;
	(*outth)=th;
}
BTexturePng* TextureUtil::getTexture(const std::string& rPath) {
	return ProjectContext::getInstance().getTexture(rPath);
}

int TextureUtil::getGridSizeX() {
	int rv=0;
	if (ProjectContext::getInstance().getNodeProject()) {
		rv=ProjectContext::getInstance().getNodeProject()->getGridSize().x;
	}
	return rv;
}
int TextureUtil::getGridSizeY() {
	int rv=0;
	if (ProjectContext::getInstance().getNodeProject()) {
		rv=ProjectContext::getInstance().getNodeProject()->getGridSize().y;
	}
	return rv;
}
