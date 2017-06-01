#pragma once
#include <QObject>
#include "nodemanager.h"


class ProjectContext
{
private:
	NodeManager		mNodeManager;

	ProjectContext();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	~ProjectContext();
	bool createNew(NodeInfoType rNodeInfoType, const string& rName, const string& rPathAbs, const string&rPathWithFileAbs);
	bool load(NodeInfoType rNodeInfoType, const string&rPathWithFileAbs);
	void save();
	void close(bool rPersistBefore);
	bool isProjectAvailable();
	bool isSceneAvailable();
	bool isResourceAvailableByName(const string &rName);
	bool isResourceAvailableByPath(const string &rPathWithFileAbs);
	string getProjectPathAbs();
	string getScenePathAbs();
	string getResourcePathAndFileAbsByName(const string& rName);
	NodeProject *getNodeProject();
	NodeScene *getNodeScene();
	NodeResource *getNodeResourceByName(const string& rName);
	NodeResource *getNodeResourceByPath(const string& rPathWithFileAbs);
	void setCurrentResource(NodeResource *rNodeResource);
	NodeResource* getCurrentResource();
};

