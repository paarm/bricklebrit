#pragma once
#include <bricklelib.h>
#include <QObject>

class ProjectContext
{
private:
	NodeProject				*mNodeProject=nullptr;
	NodeScene				*mNodeCurrentScene=nullptr;
	vector<NodeScene*>		mNodeSceneRefMap;
	string					mProjectPathAbs;
	string					mProjectPathWithFileAbs;

	string					mCurrentScenePathAbs;
	string					mCurrentScenePathWithFileAbs;
	ProjectContext();
	void closeProject();
	void closeCurrentScene();

public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	~ProjectContext();
	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);

	bool createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs);
	bool loadCurrentScene(const string&rScenePathWithFileAbs);

	string getProjectPathAbs();
	string getCurrentScenePathAbs();
	NodeProject *getNodeProject();
	NodeScene *getNodeCurrentScene();
};

