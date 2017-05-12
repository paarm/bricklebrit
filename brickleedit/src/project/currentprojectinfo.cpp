#include "currentprojectinfo.h"

CurrentProjectInfo::~CurrentProjectInfo() {
	closeProject(true);
}

void CurrentProjectInfo::closeProject(bool rPersistBefore) {
	if (mNodeProject) {
		if (rPersistBefore) {
			Node::persistNode(mNodeProject, mPathInfo.pathWithFileAbs);
		}
		delete mNodeProject;
		mNodeProject=nullptr;
	}
	NodeIdGenerator::getInstance().resetNumber();
	mCurrentSceneInfo.closeScene(rPersistBefore);
	mPathInfo.clear();
}

bool CurrentProjectInfo::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs) {
	bool rv=false;
	closeProject(true);
	mPathInfo.pathAbs=rProjectPathAbs;
	mPathInfo.pathWithFileAbs=rProjectPathWithFileAbs;

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

bool CurrentProjectInfo::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=false;
	closeProject(true);
	mPathInfo.pathWithFileAbs=rProjectPathWithFileAbs;

	QFileInfo fi(QString::fromStdString(mPathInfo.pathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mPathInfo.pathAbs=path.toStdString();
		Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
		if (rNode) {
			if (rNode->getNodeType()==NodeType::Project) {
				mNodeProject=static_cast<NodeProject*>(rNode);
				rv=true;
			} else {
				closeProject(false);
			}
		}
	}
	return rv;
}

string CurrentProjectInfo::getProjectPathAbs() {
	return mPathInfo.pathAbs;
}

string	CurrentProjectInfo::getVirtualProjectPath() {
	return "project:://";
}

NodeProject *CurrentProjectInfo::getNodeProject() {
	return mNodeProject;
}

CurrentSceneInfo& CurrentProjectInfo::getCurrentSceneInfo() {
	return mCurrentSceneInfo;
}



