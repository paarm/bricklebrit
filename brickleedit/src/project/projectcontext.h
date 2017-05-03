#pragma once
#include <bricklelib.h>
#include <QObject>

class ProjectContext
{
private:
	NodeProject				*mNodeProject=nullptr;
	NodeRoot				*currentSceneRoot=nullptr;
	vector<NodeScene>		mNodeMap;
	string					mProjectPathAbs;
	string					mProjectPathWithFileAbs;
	ProjectContext();
	void closeProject();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	~ProjectContext();
	bool createNewScene(const string& rSceneName, const string& rScenePathAbs, const string&rScenePathWithFileAbs);
	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);

	string getProjectPathAbs();
	NodeProject *getNodeProject();
};

