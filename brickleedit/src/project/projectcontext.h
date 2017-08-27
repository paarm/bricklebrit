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
	bool load(NodeInfoType rNodeInfoType, const string& rProjectPathAbs, const string& rName);
	void save();
	void closeProject(bool rPersistBefore);
	bool isProjectAvailable();
	bool isSceneAvailable();
	bool isResourceAvailableByName(const string &rName);
	string getProjectPathAbs();
	NodeProject *getNodeProject();
	NodeScene *getNodeScene();
	NodeResource *getNodeResourceByName(const string& rName);
	void setCurrentResource(NodeResource *rNodeResource);
	NodeResource* getCurrentResource();
	BTexturePng *getTexture(const string &rPathRelativeToProject);
};

