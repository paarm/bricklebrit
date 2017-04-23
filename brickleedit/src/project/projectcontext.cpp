#include "projectcontext.h"

ProjectContext::ProjectContext()
{

}

void ProjectContext::newProject() {
	mNodeRoot.deleteChildNodes();
	NodeProject* rNodeProject=static_cast<NodeProject*>(mNodeRoot.addChildNode(new NodeProject()));
	rNodeProject->setProjectName("My new Project");
	rNodeProject->addChildNode(new NodeSprite());
	string mm;
	rNodeProject->serialize(mm,0);
	cout << mm;
}
