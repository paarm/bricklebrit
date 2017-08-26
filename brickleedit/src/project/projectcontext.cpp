#include "projectcontext.h"

ProjectContext::ProjectContext() {
}

ProjectContext::~ProjectContext() {
}

void ProjectContext::save() {
	mNodeManager.save(NodeInfoType::Project);
	mNodeManager.save(NodeInfoType::Scene);
	mNodeManager.save(NodeInfoType::Resource);
}

void ProjectContext::closeProject(bool rPersistBefore) {
	if (rPersistBefore) {
		save();
	}
	mNodeManager.close(NodeInfoType::Project);
	mTextureManager.clear();
}

bool ProjectContext::load(NodeInfoType rNodeInfoType, const string&rProjectPathWithFileAbs) {
	bool rv=false;
	mTextureManager.clear();

	rv=mNodeManager.load(rNodeInfoType, rProjectPathWithFileAbs);
	if (rv) {
		mTextureManager.setBasePath(this->getProjectPathAbs());
	}
	return rv;
}


bool ProjectContext::createNew(NodeInfoType rNodeInfoType, const string& rName, const string& rPathAbs, const string &rPathWithFileAbs) {
	bool rv=false;
	rv=mNodeManager.createNew(rNodeInfoType, rName, rPathAbs, rPathWithFileAbs);
	if (rv) {
		mTextureManager.setBasePath(this->getProjectPathAbs());
	}
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

bool ProjectContext::isResourceAvailableByPath(const string &rPathWithFileAbs) {
	return mNodeManager.getNodeResourceByPath(rPathWithFileAbs);
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

NodeResource *ProjectContext::getNodeResourceByPath(const string& rPathWithFileAbs) {
	return mNodeManager.getNodeResourceByPath(rPathWithFileAbs);
}

string ProjectContext::getProjectPathAbs() {
	return mNodeManager.getProjectPathAbs();
}

string ProjectContext::getScenePathAbs() {
	return mNodeManager.getScenePathAbs();
}

string ProjectContext::getResourcePathAndFileAbsByName(const string& rName) {
	return mNodeManager.getResourcePathAndFileAbsByName(rName);
}

void ProjectContext::setCurrentResource(NodeResource *rNodeResource) {
	mNodeManager.setCurrentResource(rNodeResource);
}

NodeResource* ProjectContext::getCurrentResource() {
	return mNodeManager.getCurrentResource();
}
string ProjectContext::getCurrentResourcePathAbs() {
	return mNodeManager.getCurrentResourcePathAbs();
}

BTexturePng *ProjectContext::getTexture(const string &rPathRelativeToProject) {
	return mTextureManager.getTexture(rPathRelativeToProject);
}

