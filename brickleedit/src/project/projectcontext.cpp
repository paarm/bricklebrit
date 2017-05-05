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
		mNodeProject=nullptr;
	}
	NodeIdGenerator::getInstance().resetNumber();
	closeCurrentScene();
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

string ProjectContext::getCurrentScenePathAbs() {
	return mCurrentScenePathAbs;
}

NodeScene *ProjectContext::getNodeCurrentScene() {
	return mNodeCurrentScene;
}

void ProjectContext::closeCurrentScene() {
	if (mNodeCurrentScene) {
		Node::persistNode(mNodeCurrentScene, mCurrentScenePathWithFileAbs);
		delete mNodeCurrentScene;
		mNodeCurrentScene=nullptr;
	}
	mNodeSceneRefMap.clear();
	mNodeSceneRefMap.shrink_to_fit();
}

bool ProjectContext::createNewScene(const string& rSceneName, const string& rScenePathAbs, const string &rScenePathWithFileAbs) {
	bool rv=false;
	closeCurrentScene();

	mCurrentScenePathAbs=rScenePathAbs;
	mCurrentScenePathWithFileAbs=rScenePathWithFileAbs;
	mNodeCurrentScene=new NodeScene();
	mNodeCurrentScene->setName(rSceneName);
	rv=Node::persistNode(mNodeCurrentScene, rScenePathWithFileAbs);
	return rv;
}

bool ProjectContext::loadCurrentScene(const string&rScenePathWithFileAbs) {
	bool rv=false;
	closeCurrentScene();
	mCurrentScenePathWithFileAbs=rScenePathWithFileAbs;

	QFileInfo fi(QString::fromStdString(mCurrentScenePathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mCurrentScenePathAbs=path.toStdString();
		Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
		if (rNode) {
			if (rNode->getNodeType()==NodeType::Scene) {
				mNodeCurrentScene=static_cast<NodeScene*>(rNode);
				rv=true;
			} else {
				closeCurrentScene();
			}
		}
	}
	return rv;
}


