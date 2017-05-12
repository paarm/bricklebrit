#include "currentsceneinfo.h"

CurrentSceneInfo::~CurrentSceneInfo() {
	closeScene(true);
}

void CurrentSceneInfo::closeScene(bool rPersistBefore) {
	if (mNodeScene) {
		if (rPersistBefore) {
			Node::persistNode(mNodeScene, mPathInfo.pathWithFileAbs);
		}
		delete mNodeScene;
		mNodeScene=nullptr;
	}
	mNodeSceneRefMap.clear();
	mNodeSceneRefMap.shrink_to_fit();
	mPathInfo.clear();
}

string CurrentSceneInfo::getScenePathAbs() {
	return mPathInfo.pathAbs;
}

NodeScene* CurrentSceneInfo::getNodeScene() {
	return mNodeScene;
}

bool CurrentSceneInfo::createNewScene(const string& rSceneName, const string& rScenePathAbs, const string &rScenePathWithFileAbs) {
	bool rv=false;
	closeScene(true);

	mPathInfo.pathAbs=rScenePathAbs;
	mPathInfo.pathWithFileAbs=rScenePathWithFileAbs;
	mNodeScene=new NodeScene();
	mNodeScene->setName(rSceneName);
	rv=Node::persistNode(mNodeScene, mPathInfo.pathWithFileAbs);
	return rv;
}

bool CurrentSceneInfo::loadScene(const string&rScenePathWithFileAbs) {
	bool rv=false;
	closeScene(true);
	mPathInfo.pathWithFileAbs=rScenePathWithFileAbs;

	QFileInfo fi(QString::fromStdString(mPathInfo.pathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mPathInfo.pathAbs=path.toStdString();
		Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
		if (rNode) {
			if (rNode->getNodeType()==NodeType::Scene) {
				mNodeScene=static_cast<NodeScene*>(rNode);
				rv=true;
			} else {
				closeScene(false);
			}
		}
	}
	return rv;
}

