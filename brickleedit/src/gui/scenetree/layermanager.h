#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H
#include <bricklelib.h>

class LayerManager
{
private:
	NodeLayer *mCurrentLayer=nullptr;
public:
	LayerManager();
	void currentSceneChanged();
	void setCurrentLayer(NodeLayer *rNodeLayer);
	int getCurrentLayerIndex(size_t *rOutLayerCount);
	NodeLayer* getCurrentLayer();
	void addNewLayer();
	void deleteCurrentLayer();
	bool moveCurrentLayerUpOrDown(bool rMoveUp);
	void setLayerVisible(NodeLayer *rNodeLayer, bool rIsVisible);
	void setLayerLocked(NodeLayer *rNodeLayer, bool rIsLocked);
};

#endif // LAYERMANAGER_H
