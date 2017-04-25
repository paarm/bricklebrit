#pragma once
#include <bricklelib.h>
#include <QObject>

class ProjectContext
{
private:
	NodeRoot		mNodeRoot;
	string			mProjectPathAbs;
	string			mProjectPathWithFileAbs;
	ProjectContext();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	bool createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string&rProjectPathWithFileAbs);
	bool loadProject(const string&rProjectPathWithFileAbs);

	string getProjectPathAbs();
	NodeProject *getNodeProject();
};

