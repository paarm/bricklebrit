#include "projectcontext.h"
#include <QFileInfo>

ProjectContext::ProjectContext() {
}

ProjectContext::~ProjectContext() {
	closeProject();
}

void ProjectContext::closeProject() {
	if (mNodeProject) {
		delete mNodeProject;
	}
	NodeIdGenerator::getInstance().resetNumber();
}

bool ProjectContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathAbs=rProjectPathAbs;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	closeProject();
	mNodeProject=new NodeProject();
	mNodeProject->setProjectName(rProjectName);

	PropertyList *pl=mNodeProject->getPropertyListListTest();
	pl->addEntry(new PropertyString("Test 1"));
	pl->addEntry(new PropertyString("Test 2"));
	pl->addEntry(new PropertyRectFloat(10.0,20.0,11.0,12.0));

	Node *atlas=mNodeProject->addChildNode(new NodeTextureAtlas());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());

	Node *animation=mNodeProject->addChildNode(new NodeAnimationSet());
	animation->addChildNode(new NodeAnimationSetFrameTexture);
	animation->addChildNode(new NodeAnimationSetFrameTexture);
	animation->addChildNode(new NodeAnimationSetFrameTextureAtlas);
	animation->addChildNode(new NodeAnimationSetFrameTextureAtlas);

	mNodeProject->addChildNode(new NodeSprite());
	rv=Node::persistNode(mNodeProject, rProjectPathWithFileAbs);
	return rv;
}

bool ProjectContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	closeProject();

	QFileInfo fi(QString::fromStdString(mProjectPathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mProjectPathAbs=path.toStdString();
		Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
		if (rNode) {
			if (rNode->getNodeType()==NodeType::Project) {
				mNodeProject=static_cast<NodeProject*>(rNode);
				rv=true;
			} else {
				closeProject();
			}
		}
	}
	return rv;
}

string ProjectContext::getProjectPathAbs() {
	return mProjectPathAbs;
}

NodeProject *ProjectContext::getNodeProject() {
	return mNodeProject;
}

