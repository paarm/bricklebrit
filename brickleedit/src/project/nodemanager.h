#pragma once
#include <bricklelib.h>

class NodeManager
{
private:
	NodeProject						*mNodeProject=nullptr;
	vector<NodeResource*>			mNodeResourceList;
	vector<NodeScene*>				mNodeSceneList;

	Node* loadNode(const string&rPathWithFileAbs);
public:
	NodeManager();
	~NodeManager();
	bool createNewProject(const string& rName);
	bool createNewScene(const string& rName);
	bool createNewResource(const string& rName);
	bool loadProjectNode(const string& rProjectPathAbs, const string &rName);
	bool loadSceneNode(const string& rProjectPathAbs, const string &rName);
	bool loadResourceNode(const string& rProjectPathAbs, const string &rName);
	void saveNodes(const string& rProjectPathAbs);
	void closeNodes();
	NodeProject *getNodeProject();
	NodeScene * getNodeSceneByName(const string &rName);
	NodeResource * getNodeResourceByName(const string &rName);
	string getProjectPathAbs();
};

