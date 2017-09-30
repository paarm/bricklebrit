#include "projectcontext.h"

ProjectContext::ProjectContext() {
}

ProjectContext::~ProjectContext() {
}

void ProjectContext::save() {
	mNodeManager.saveNodes(mProjectPathAbs);
}

void ProjectContext::closeProject(bool rPersistBefore) {
	if (rPersistBefore) {
		save();
	}
	mNodeManager.closeNodes();
	mProjectPathAbs="";
	mTextureManager.clear();
}

bool ProjectContext::loadProject(const string&rProjectPathAbs, const string&rName) {
	bool rv=false;

	rv=mNodeManager.loadProjectNode(rProjectPathAbs, rName);
	if (rv) {
		mTextureManager.clear();
		mProjectPathAbs=rProjectPathAbs;
		mTextureManager.setBasePath(this->getProjectPathAbs());
	}
	return rv;
}

bool ProjectContext::loadScene(const string&rName) {
	bool rv=false;
	rv=mNodeManager.loadSceneNode(mProjectPathAbs, rName);
	return rv;
}

bool ProjectContext::loadResource(const string&rName) {
	bool rv=false;
	rv=mNodeManager.loadResourceNode(mProjectPathAbs, rName);
	return rv;
}

bool ProjectContext::createNewProject(const string& rProjectPathAbs, const string& rName) {
	bool rv=false;
	rv=mNodeManager.createNewProject(rName);
	if (rv) {
		mProjectPathAbs=rProjectPathAbs;
		mTextureManager.setBasePath(this->getProjectPathAbs());
	}
	return rv;
}

bool ProjectContext::createNewScene(const string& rName) {
	bool rv=false;
	rv=mNodeManager.createNewScene(rName);
	return rv;
}

bool ProjectContext::createNewResource(const string& rName) {
	bool rv=false;
	rv=mNodeManager.createNewResource(rName);
	return rv;
}

bool ProjectContext::isProjectAvailable() {
	return mNodeManager.getNodeProject();
}

NodeProject *ProjectContext::getNodeProject() {
	return mNodeManager.getNodeProject();
}

NodeResource *ProjectContext::getNodeResourceByName(const string& rName) {
	return mNodeManager.getNodeResourceByName(rName);
}

NodeScene *ProjectContext::getNodeSceneByName(const string& rName) {
	return mNodeManager.getNodeSceneByName(rName);
}

string ProjectContext::getProjectPathAbs() {
	return mProjectPathAbs;
}

BTexturePng *ProjectContext::getTexture(const string &rPathRelativeToProject) {
	return mTextureManager.getTexture(rPathRelativeToProject);
}

vector<string> ProjectContext::getSceneNames() {
	vector<string> rv;
	if (mNodeManager.getNodeProject()) {
		vector<Node*> v=mNodeManager.getNodeProject()->getChildNodesWithNodeType(NodeType::SceneInfo);
		for (auto rNode : v) {
			NodeSceneInfo *rNodeSceneInfo=static_cast<NodeSceneInfo*>(rNode);
			rv.push_back(rNodeSceneInfo->getPath());
		}
	}
	return rv;
}

vector<string> ProjectContext::getResourceNames() {
	vector<string> rv;
	if (mNodeManager.getNodeProject()) {
		vector<Node*> v=mNodeManager.getNodeProject()->getChildNodesWithNodeType(NodeType::ResourceInfo);
		for (auto rNode : v) {
			NodeResourceInfo *rNodeResourceInfo=static_cast<NodeResourceInfo*>(rNode);
			rv.push_back(rNodeResourceInfo->getPath());
		}
	}
	return rv;
}

NodeSceneInfo* ProjectContext::getSceneInfoByName(const string& rSceneName) {
	NodeSceneInfo *rNodeSceneInfo=nullptr;
	if (getNodeProject()) {
		Node *rNode=getNodeProject()->getChildNodeWithNameAndNodeType(rSceneName, NodeType::SceneInfo);
		if (rNode && rNode->getNodeType()==NodeType::SceneInfo) {
			rNodeSceneInfo=static_cast<NodeSceneInfo*>(rNode);
		}
	}
	return rNodeSceneInfo;
}

NodeScene* ProjectContext::getOrLoadSceneByName(const string& rSceneName) {
	NodeScene *rNodeScene=nullptr;
	if (getSceneInfoByName(rSceneName)) {
		rNodeScene=getNodeSceneByName(rSceneName);
		if (!rNodeScene) {
			if (loadScene(rSceneName)) {
				rNodeScene=getNodeSceneByName(rSceneName);
			}
		}
	}
	return rNodeScene;
}

NodeResourceInfo* ProjectContext::getResourceInfoByName(const string& rResourceName) {
	NodeResourceInfo *rNodeResourceInfo=nullptr;
	if (getNodeProject()) {
		Node *rNode=getNodeProject()->getChildNodeWithNameAndNodeType(rResourceName, NodeType::ResourceInfo);
		if (rNode && rNode->getNodeType()==NodeType::ResourceInfo) {
			rNodeResourceInfo=static_cast<NodeResourceInfo*>(rNode);
		}
	}
	return rNodeResourceInfo;
}

NodeResource* ProjectContext::getOrLoadResourceByName(const string& rResourceName) {
	NodeResource *rNodeResource=nullptr;
	if (getResourceInfoByName(rResourceName)) {
		rNodeResource=getNodeResourceByName(rResourceName);
		if (!rNodeResource) {
			if (loadResource(rResourceName)) {
				rNodeResource=getNodeResourceByName(rResourceName);
			}
		}
	}
	return rNodeResource;
}

NodeScene* ProjectContext::getDefaultScene() {
	NodeScene* rNodeScene=nullptr;
	if (getNodeProject()) {
		vector<Node*> v=getNodeProject()->getChildNodesWithNodeType(NodeType::SceneInfo);
		for(auto rNode :  v) {
			if (rNode->getNodeType()==NodeType::SceneInfo) {
				NodeSceneInfo* rNodeSceneInfo=static_cast<NodeSceneInfo*>(rNode);
				if (rNodeSceneInfo->getIsDefault()) {
					rNodeScene=getOrLoadSceneByName(rNodeSceneInfo->getName());
					break;
				}
			}
		}
	}
	return rNodeScene;
}

NodeResource* ProjectContext::getDefaultResource() {
	NodeResource* rNodeResource=nullptr;
	if (getNodeProject()) {
		vector<Node*> v=getNodeProject()->getChildNodesWithNodeType(NodeType::ResourceInfo);
		for(auto rNode :  v) {
			if (rNode->getNodeType()==NodeType::ResourceInfo) {
				NodeResourceInfo* rNodeResourceInfo=static_cast<NodeResourceInfo*>(rNode);
				if (rNodeResourceInfo->getIsDefault()) {
					rNodeResource=getOrLoadResourceByName(rNodeResourceInfo->getName());
					break;
				}
			}
		}
	}
	return rNodeResource;
}
