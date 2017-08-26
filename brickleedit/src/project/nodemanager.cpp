#include "nodemanager.h"

NodeManager::NodeManager() {
}

NodeManager::~NodeManager() {
	close(NodeInfoType::Project);
	close(NodeInfoType::Scene);
	close(NodeInfoType::Resource);
}

bool NodeManager::createNew(NodeInfoType rNodeInfoType, const string& rName, const string& rPathAbs, const string&rPathWithFileAbs) {
	switch (rNodeInfoType) {
	case NodeInfoType::Project:
		NodeIdGenerator::getInstance().resetNumber(0);
		return mNodeProject.createNew(rName, rPathAbs, rPathWithFileAbs);
	case NodeInfoType::Scene:
		return mNodeScene.createNew(rName, rPathAbs, rPathWithFileAbs);
	case NodeInfoType::Resource:
		mNodeResource.emplace_back(3);
		NodeInfo<NodeResource>& rNodeResource=mNodeResource.back();
		return rNodeResource.createNew(rName, rPathAbs, rPathWithFileAbs);
	}
	return false;
}

bool NodeManager::load(NodeInfoType rNodeInfoType, const string&rPathWithFileAbs) {
	switch (rNodeInfoType) {
	case NodeInfoType::Project:
		if (mNodeProject.load(rPathWithFileAbs)) {
			if (mNodeProject.getNode() && mNodeProject.getNode()->getNodeType()==NodeType::Project) {
				NodeIdGenerator::getInstance().resetNumber(mNodeProject.getNode()->getNextFreeId());
				return true;
			}
		}
		close(rNodeInfoType);
		break;
	case NodeInfoType::Scene:
		if (mNodeScene.load(rPathWithFileAbs)) {
			if (mNodeScene.getNode() && mNodeScene.getNode()->getNodeType()==NodeType::Scene) {
				return true;
			}
		}
		close(rNodeInfoType);
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			if (r.getPathWithFileAbs()==rPathWithFileAbs) {
				// already loaded
				return true;
			}
		}
		//NodeInfo<NodeResource> rNew;
		mNodeResource.emplace_back(3);
		NodeInfo<NodeResource>& rNodeInfoResource=mNodeResource.back();
		if (rNodeInfoResource.load(rPathWithFileAbs)) {
			if (rNodeInfoResource.getNode() && rNodeInfoResource.getNode()->getNodeType()==NodeType::Resource) {
				return true;
			}
		}
		mNodeResource.pop_back();
	}
	return false;
}

void NodeManager::close(NodeInfoType rNodeInfoType) {
	switch (rNodeInfoType) {
	case NodeInfoType::Project:
		mNodeProject.close();
		NodeIdGenerator::getInstance().resetNumber();

		close(NodeInfoType::Scene);
		close(NodeInfoType::Resource);
		break;
	case NodeInfoType::Scene:
		mNodeScene.close();
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			r.close();
		}
		mNodeResource.clear();
		mNodeResource.shrink_to_fit();
		setCurrentResource(nullptr);
		break;
	}
}

void NodeManager::save(NodeInfoType rNodeInfoType) {
	switch (rNodeInfoType) {
	case NodeInfoType::Project:
		mNodeProject.getNode()->setNextFreeId(NodeIdGenerator::getInstance().getNextFreeNumber());
		mNodeProject.save();
		break;
	case NodeInfoType::Scene:
		mNodeScene.save();
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			r.save();
		}
		break;
	}
}

string NodeManager::getProjectPathAbs() {
	return mNodeProject.getPathAbs();
}

string NodeManager::getScenePathAbs() {
	return mNodeScene.getPathAbs();
}


string NodeManager::getResourcePathAndFileAbsByName(const string &rName) {
	string rv="";
	for (auto& r : mNodeResource) {
		if (r.getNode()->getName()==rName) {
			rv=r.getPathWithFileAbs();
			break;
		}
	}
	return rv;
}


NodeProject *NodeManager::getNodeProject() {
	return mNodeProject.getNode();
}

NodeScene *NodeManager::getNodeScene() {
	return mNodeScene.getNode();
}

NodeResource *NodeManager::getNodeResourceByName(const string &rName) {
	NodeResource *rv=nullptr;
	for (auto& r : mNodeResource) {
		if (r.getNode()->getName()==rName) {
			rv=r.getNode();
			break;
		}
	}
	return rv;
}

NodeResource *NodeManager::getNodeResourceByPath(const string &rPathWithFileAbs) {
	NodeResource *rv=nullptr;
	for (auto& r : mNodeResource) {
		if (r.getPathWithFileAbs()==rPathWithFileAbs) {
			rv=r.getNode();
			break;
		}
	}
	return rv;
}

void NodeManager::setCurrentResource(NodeResource *rNodeResource) {
	mNodeResourceCurrent=rNodeResource;
	mNodeResourceCurrentInfo=nullptr;
	for (auto& r : mNodeResource) {
		if (r.getNode()==rNodeResource) {
			mNodeResourceCurrentInfo=&r;
		}
	}
}

NodeResource* NodeManager::getCurrentResource() {
	return mNodeResourceCurrent;
}

string NodeManager::getCurrentResourcePathAbs() {
	string rv="";
	if (mNodeResourceCurrentInfo) {
		rv=mNodeResourceCurrentInfo->getPathAbs();
	}
	return rv;
}


