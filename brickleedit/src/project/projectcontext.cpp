#include "projectcontext.h"

ProjectContext::ProjectContext() {
}

ProjectContext::~ProjectContext() {
}

void ProjectContext::save() {
	mNodeManager.save(NodeInfoType::Project,mProjectPathAbs);
	mNodeManager.save(NodeInfoType::Scene,mProjectPathAbs);
	mNodeManager.save(NodeInfoType::Resource,mProjectPathAbs);
}

void ProjectContext::closeProject(bool rPersistBefore) {
	if (rPersistBefore) {
		save();
	}
	mNodeManager.close(NodeInfoType::Project);
	mProjectPathAbs="";
	mTextureManager.clear();
}

bool ProjectContext::load(NodeInfoType rNodeInfoType, const string&rProjectPathAbs, const string&rName) {
	bool rv=false;

	rv=mNodeManager.load(rNodeInfoType, rProjectPathAbs, rName);
	if (rv) {
		if (rNodeInfoType==NodeInfoType::Project) {
			mTextureManager.clear();
			mProjectPathAbs=rProjectPathAbs;
			mTextureManager.setBasePath(this->getProjectPathAbs());
		}
	}
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

bool ProjectContext::createNew(NodeInfoType rNodeInfoType, const string& rName) {
	bool rv=false;
	rv=mNodeManager.createNew(rNodeInfoType, rName);
	return rv;
}

bool ProjectContext::isProjectAvailable() {
	return mNodeManager.getNodeProject();
}

bool ProjectContext::isSceneAvailable() {
	return mNodeManager.getNodeScene();
}

bool ProjectContext::isResourceAvailableByName(const string &rName) {
	return mNodeManager.getNodeResourceByName(rName);
}

NodeProject *ProjectContext::getNodeProject() {
	return mNodeManager.getNodeProject();
}

NodeScene *ProjectContext::getNodeScene() {
	return mNodeManager.getNodeScene();
}

NodeResource *ProjectContext::getNodeResourceByName(const string& rName) {
	return mNodeManager.getNodeResourceByName(rName);
}

string ProjectContext::getProjectPathAbs() {
	return mProjectPathAbs;
}

void ProjectContext::setCurrentResource(NodeResource *rNodeResource) {
	mNodeManager.setCurrentResource(rNodeResource);
}

NodeResource* ProjectContext::getCurrentResource() {
	return mNodeManager.getCurrentResource();
}

BTexturePng *ProjectContext::getTexture(const string &rPathRelativeToProject) {
	return mTextureManager.getTexture(rPathRelativeToProject);
}

