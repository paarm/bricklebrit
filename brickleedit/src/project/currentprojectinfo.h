#pragma once
#include "currentsceneinfo.h"


class CurrentProjectInfo {
private:
	PathInfo				mPathInfo;
	NodeProject				*mNodeProject=nullptr;
	CurrentSceneInfo		mCurrentSceneInfo;
public:
	virtual ~CurrentProjectInfo();
	void	closeProject(bool rPersistBefore);
	bool	createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs);
	bool	loadProject(const string&rProjectPathWithFileAbs);
	string	getProjectPathAbs();
	NodeProject *getNodeProject();
	CurrentSceneInfo& getCurrentSceneInfo();
};
