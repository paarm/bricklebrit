#pragma once
#include "nodeinfo.h"
#include "nodeinfotype.h"

class NodeManager
{
private:
	NodeInfo<NodeProject>			mNodeProject{1};
	NodeInfo<NodeScene>				mNodeScene{2};
	vector<NodeInfo<NodeResource>>	mNodeResource;
	NodeResource*					mNodeResourceCurrent=nullptr;
    NodeInfo<NodeResource>          *mNodeResourceCurrentInfo=nullptr;
public:
	NodeManager();
	~NodeManager();
	bool createNewProject(const string& rName);
	bool createNew(NodeInfoType rNodeInfoType, const string& rName);
	bool load(NodeInfoType rNodeInfoType, const string& rProjectPathAbs, const string &rName);
	void save(NodeInfoType rNodeInfoType, const string& rProjectPathAbs);
	void close(NodeInfoType rNodeInfoType);
	NodeProject *getNodeProject();
	NodeScene *getNodeScene();
	NodeResource * getNodeResourceByName(const string &rName);
	string getProjectPathAbs();
	void setCurrentResource(NodeResource *rNodeResource);
	NodeResource* getCurrentResource();
};

