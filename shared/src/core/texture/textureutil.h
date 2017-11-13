#ifndef TEXTUREUTIL_H
#define TEXTUREUTIL_H

class Node2d;
class NodeTexture;
class NodeTextureFrame;
class BTexturePng;

#include <string>

class TextureUtil
{
public:
	static void getTexture(Node2d *paintNode, NodeTexture **rOutNodeTexture, NodeTextureFrame **rOutNodeTextureFrame);
	static void getTextureCoords(Node2d* rNodeSprite, NodeTextureFrame *rNodeTextureFrame, BTexturePng *bTexture, float *outtx, float *outty, float *outtw, float* outth);
	static BTexturePng* getTexture(const std::string& rPath);
	static int getGridSizeX();
	static int getGridSizeY();
};

#endif // TEXTUREUTIL_H
