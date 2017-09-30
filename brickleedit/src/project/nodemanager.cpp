#include "nodemanager.h"
#include <QFileInfo>
NodeManager::NodeManager() {
}

NodeManager::~NodeManager() {
	closeNodes();
}

bool NodeManager::createNewProject(const string& rName) {
	closeNodes();
	NodeIdGenerator::getInstance().resetNumber(0);
	mNodeProject=static_cast<NodeProject*>(getInstanceFromNodeType(NodeType::Project, true));
	return true;
}

bool NodeManager::createNewScene(const string& rName) {
	NodeScene* rNodeScene=static_cast<NodeScene*>(getInstanceFromNodeType(NodeType::Scene, true));
	mNodeSceneList.push_back(rNodeScene);
	return true;
}

bool NodeManager::createNewResource(const string& rName) {
	NodeResource* rNodeResource=static_cast<NodeResource*>(getInstanceFromNodeType(NodeType::Resource, true));
	mNodeResourceList.push_back(rNodeResource);
	return true;
}

Node* NodeManager::loadNode(const string&rPathWithFileAbs) {
	Node *rv=nullptr;
	QFileInfo fi(QString::fromStdString(rPathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		rv=Node::unpersistNode(fi.absoluteFilePath().toStdString());
	}
	return rv;
}

bool NodeManager::loadProjectNode(const string& rProjectPathAbs, const string &rName) {
	bool rv=false;
	string rPathWithFileAbs=DirUtil::concatPath(rProjectPathAbs, rName);

	if (mNodeProject) {
		closeNodes();
	}
	mNodeProject=static_cast<NodeProject*>(loadNode(rPathWithFileAbs));

	if (mNodeProject && mNodeProject->getNodeType()==NodeType::Project) {
		NodeIdGenerator::getInstance().resetNumber(mNodeProject->getNextFreeId());
		rv=true;
	} else if (mNodeProject) {
		delete mNodeProject;
		mNodeProject=nullptr;
	}
	return rv;
}

bool NodeManager::loadSceneNode(const string& rProjectPathAbs, const string &rName) {
	bool rv=false;
	string rPathWithFileAbs=DirUtil::concatPath(rProjectPathAbs, rName);

	for (NodeScene* r : mNodeSceneList) {
		if (r->getName()==rName) {
			// already loaded
			return true;
		}
	}
	NodeScene* rNodeScene=static_cast<NodeScene*>(loadNode(rPathWithFileAbs));
	if (rNodeScene && rNodeScene->getNodeType()==NodeType::Scene) {
		mNodeSceneList.push_back(rNodeScene);
		rv=true;
	} else if (rNodeScene) {
		delete rNodeScene;
	}
	return rv;
}

bool NodeManager::loadResourceNode(const string& rProjectPathAbs, const string &rName) {
	bool rv=false;
	string rPathWithFileAbs=DirUtil::concatPath(rProjectPathAbs, rName);

	for (auto r : mNodeResourceList) {
		if (r->getName()==rName) {
			// already loaded
			return true;
		}
	}
	NodeResource* rNodeResource=static_cast<NodeResource*>(loadNode(rPathWithFileAbs));
	if (rNodeResource && rNodeResource->getNodeType()==NodeType::Resource) {
		mNodeResourceList.push_back(rNodeResource);
		rv=true;
	} else if (rNodeResource) {
		delete rNodeResource;
	}
	return rv;
}


void NodeManager::closeNodes() {
	if (mNodeProject) {
		delete mNodeProject;
		mNodeProject=nullptr;

		for (NodeScene* r : mNodeSceneList) {
			delete r;
		}
		mNodeSceneList.clear();
		mNodeSceneList.shrink_to_fit();

		for (NodeResource* r: mNodeResourceList) {
			delete r;
		}
		mNodeResourceList.clear();
		mNodeResourceList.shrink_to_fit();
	}
	NodeIdGenerator::getInstance().resetNumber();
}

void NodeManager::saveNodes(const string& rProjectPathAbs) {
	if (mNodeProject) {
		mNodeProject->setNextFreeId(NodeIdGenerator::getInstance().getNextFreeNumber());
		Node::persistNode(mNodeProject, DirUtil::concatPath(rProjectPathAbs, mNodeProject->getName()));

		for (auto r : mNodeSceneList) {
			Node::persistNode(r, DirUtil::concatPath(rProjectPathAbs, r->getName()));
		}
		for (auto r : mNodeResourceList) {
			Node::persistNode(r, DirUtil::concatPath(rProjectPathAbs, r->getName()));
		}
	}
}

NodeProject *NodeManager::getNodeProject() {
	return mNodeProject;
}

NodeResource *NodeManager::getNodeResourceByName(const string &rName) {
	NodeResource *rv=nullptr;
	for (NodeResource *r : mNodeResourceList) {
		if (r->getName()==rName) {
			rv=r;
			break;
		}
	}
	return rv;
}

NodeScene *NodeManager::getNodeSceneByName(const string &rName) {
	NodeScene *rv=nullptr;
	for (NodeScene* r : mNodeSceneList) {
		if (r->getName()==rName) {
			rv=r;
			break;
		}
	}
	return rv;
}


