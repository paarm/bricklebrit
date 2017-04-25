#pragma once
#include <bricklelib.h>
#include <QObject>

class ProjectContext
{
private:
	NodeRoot		mNodeRoot;
	ProjectContext();
public:
	static ProjectContext& getInstance() {
		static ProjectContext rProjectContext;
		return rProjectContext;
	}
	void createNewProject(const string& rProjectName);
};

