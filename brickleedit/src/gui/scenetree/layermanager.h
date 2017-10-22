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
	NodeLayer* getCurrentLayer();
	void addNewLayer();
	void deleteCurrentLayer();
	void moveLayerUp();
	void moveLayerDown();
	void setLayerVisible(NodeLayer *rNodeLayer, bool rIsVisible);
};

#endif // LAYERMANAGER_H
