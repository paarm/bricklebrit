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
	bool createNew(NodeInfoType rNodeInfoType, const string& rName, const string& rPathAbs, const string&rPathWithFileAbs);
	bool load(NodeInfoType rNodeInfoType, const string&rPathWithFileAbs);
	void save(NodeInfoType rNodeInfoType);
	void close(NodeInfoType rNodeInfoType);
	NodeProject *getNodeProject();
	NodeScene *getNodeScene();
	NodeResource * getNodeResourceByPath(const string &rPathWithFileAbs);
	NodeResource * getNodeResourceByName(const string &rName);
	string getProjectPathAbs();
	string getScenePathAbs();
	string getResourcePathAndFileAbsByName(const string &rName);
	void setCurrentResource(NodeResource *rNodeResource);
	NodeResource* getCurrentResource();
    string getCurrentResourcePathAbs();
};

