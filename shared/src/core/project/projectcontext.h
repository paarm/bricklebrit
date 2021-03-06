#pragma once
#include "nodemanager.h"
#include "../texture/texturemanager.h"

class ProjectContext
{
private:
	string			mProjectPathAbs;
	NodeManager		mNodeManager;
	TextureManager	mTextureManager;

	ProjectContext();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	~ProjectContext();
	bool createNewProject(const string& rProjectPathAbs, const string& rName);
	bool createNewScene(const string& rName);
	bool createNewResource(const string& rName);
	bool loadProject(const string& rProjectPathAbs, const string& rName);
	bool loadResource(const string& rName);
	bool loadScene(const string& rName);
	void save();
	void closeProject(bool rPersistBefore);
	bool isProjectAvailable();
	string getProjectPathAbs();
	NodeProject *getNodeProject();
	NodeResource *getNodeResourceByName(const string& rName);
	NodeResource *getDefaultResource();
	NodeSceneInfo* getSceneInfoByName(const string& rSceneName);
	NodeScene* getOrLoadSceneByName(const string& rSceneName);
	NodeResourceInfo* getResourceInfoByName(const string& rResourceName);
	NodeResource* getOrLoadResourceByName(const string& rResourceName);
	NodeScene *getNodeSceneByName(const string& rName);
	NodeScene *getDefaultScene();
	vector<string> getChildNodeNamesWithType(NodeType rNodeType);
	vector<string> getSceneNames();
	vector<string> getResourceNames();
	BTexturePng *getTexture(const string &rPathRelativeToProject);
};

