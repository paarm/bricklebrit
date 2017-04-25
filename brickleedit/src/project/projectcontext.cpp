#include "projectcontext.h"
#include <QFileInfo>

ProjectContext::ProjectContext() {}

bool ProjectContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathAbs=rProjectPathAbs;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	mNodeRoot.deleteChildNodes();

	NodeProject* rNodeProject=static_cast<NodeProject*>(mNodeRoot.addChildNode(new NodeProject()));
	rNodeProject->setProjectName(rProjectName);
	rNodeProject->addChildNode(new NodeSprite());
	rv=rNodeProject->persistSubNode(rProjectPathWithFileAbs);
	return rv;
}

bool ProjectContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	mNodeRoot.deleteChildNodes();

	QFileInfo fi(QString::fromStdString(mProjectPathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mProjectPathAbs=path.toStdString();
		NodeParser rNodeParser;
		rNodeParser.parseFile(&mNodeRoot, fi.absoluteFilePath().toStdString());
		if (getNodeProject()) {
			rv=true;
		}
	}
	return rv;
}

string ProjectContext::getProjectPathAbs() {
	return mProjectPathAbs;
}

NodeProject *ProjectContext::getNodeProject() {
	Node *rNodeChild=mNodeRoot.getFirstChildNode();
	if (rNodeChild && rNodeChild->getContentType()==ContentType::Project) {
		NodeProject *rNodeProject=static_cast<NodeProject*>(rNodeChild);
		return rNodeProject;
	}
	return nullptr;
}

