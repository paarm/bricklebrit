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
	NodeScene *getNodeSceneByName(const string& rName);
	BTexturePng *getTexture(const string &rPathRelativeToProject);
};

