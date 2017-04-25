#include "projectcontext.h"

ProjectContext::ProjectContext() {}

void ProjectContext::createNewProject(const string& rProjectName) {
	mNodeRoot.deleteChildNodes();
	NodeProject* rNodeProject=static_cast<NodeProject*>(mNodeRoot.addChildNode(new NodeProject()));
	rNodeProject->setProjectName(rProjectName);
	rNodeProject->addChildNode(new NodeSprite());
	string mm;
	rNodeProject->serialize(mm,0);
	cout << mm;
}
