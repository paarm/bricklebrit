#include "nodemanager.h"

NodeManager::NodeManager() {
}

NodeManager::~NodeManager() {
	close(NodeInfoType::Project);
	close(NodeInfoType::Scene);
	close(NodeInfoType::Resource);
}

bool NodeManager::createNewProject(const string& rName) {
	NodeIdGenerator::getInstance().resetNumber(0);
	return mNodeProject.createNew(rName);
}

bool NodeManager::createNew(NodeInfoType rNodeInfoType, const string& rName) {
	switch (rNodeInfoType) {
	case NodeInfoType::Scene:
	{
			mNodeScene.emplace_back(3);
			NodeInfo<NodeScene>& rNodeScene=mNodeScene.back();
			return rNodeScene.createNew(rName);
	}
		break;
	case NodeInfoType::Resource:
		mNodeResource.emplace_back(3);
		NodeInfo<NodeResource>& rNodeResource=mNodeResource.back();
		return rNodeResource.createNew(rName);
		break;
	}
	return false;
}

bool NodeManager::load(NodeInfoType rNodeInfoType, const string& rProjectPathAbs, const string &rName) {
	string rPathWithFileAbs=DirUtil::concatPath(rProjectPathAbs, rName);
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
	{
		for (auto& r : mNodeScene) {
			if (r.getNode()->getName()==rName) {
				// already loaded
				return true;
			}
		}
		mNodeScene.emplace_back(3);
		NodeInfo<NodeScene>& rNodeInfoScene=mNodeScene.back();
		if (rNodeInfoScene.load(rPathWithFileAbs)) {
			if (rNodeInfoScene.getNode() && rNodeInfoScene.getNode()->getNodeType()==NodeType::Scene) {
				return true;
			}
		}
		mNodeScene.pop_back();
	}
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			if (r.getNode()->getName()==rName) {
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
		for (auto& r : mNodeScene) {
			r.close();
		}
		mNodeScene.clear();
		mNodeScene.shrink_to_fit();
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			r.close();
		}
		mNodeResource.clear();
		mNodeResource.shrink_to_fit();
		break;
	}
}

void NodeManager::save(NodeInfoType rNodeInfoType, const string& rProjectPathAbs) {
	switch (rNodeInfoType) {
	case NodeInfoType::Project:
		mNodeProject.getNode()->setNextFreeId(NodeIdGenerator::getInstance().getNextFreeNumber());
		mNodeProject.save(rProjectPathAbs);
		break;
	case NodeInfoType::Scene:
		for (auto& r : mNodeScene) {
			r.save(rProjectPathAbs);
		}
		break;
	case NodeInfoType::Resource:
		for (auto& r : mNodeResource) {
			r.save(rProjectPathAbs);
		}
		break;
	}
}

NodeProject *NodeManager::getNodeProject() {
	return mNodeProject.getNode();
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

NodeScene *NodeManager::getNodeSceneByName(const string &rName) {
	NodeScene *rv=nullptr;
	for (auto& r : mNodeScene) {
		if (r.getNode()->getName()==rName) {
			rv=r.getNode();
			break;
		}
	}
	return rv;
}


