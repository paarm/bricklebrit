#pragma once
#include <QObject>
#include "nodemanager.h"


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
	bool createNew(NodeInfoType rNodeInfoType, const string& rName);
	bool loadProject(const string& rProjectPathAbs, const string& rName);
	bool load(NodeInfoType rNodeInfoType, const string& rName);
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
	vector<string> getFileNamesForType(NodeInfoType rNodeInfoType);
	BTexturePng *getTexture(const string &rPathRelativeToProject);
};

