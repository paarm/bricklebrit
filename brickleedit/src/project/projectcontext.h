#pragma once
#include <QObject>
#include "currentprojectinfo.h"

class ProjectContext
{
private:
	CurrentProjectInfo		mCurrentProjectInfo;

	ProjectContext();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	~ProjectContext();
	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);
	void closeProject(bool rPersistBefore);

	bool createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs);
	bool loadCurrentScene(const string&rScenePathWithFileAbs);
	void closeCurrentScene(bool rPersistBefore);
	bool isCurrentSceneAvailable();


	string getProjectPathAbs();
	NodeProject *getNodeProject();
	string getCurrentScenePathAbs();
	NodeScene *getNodeCurrentScene();
};

