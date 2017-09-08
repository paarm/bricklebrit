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

bool ProjectContext::loadProject(const string&rProjectPathAbs, const string&rName) {
	bool rv=false;

	rv=mNodeManager.load(NodeInfoType::Project, rProjectPathAbs, rName);
	if (rv) {
		mTextureManager.clear();
		mProjectPathAbs=rProjectPathAbs;
		mTextureManager.setBasePath(this->getProjectPathAbs());
	}
	return rv;
}

bool ProjectContext::load(NodeInfoType rNodeInfoType, const string&rName) {
	bool rv=false;
	rv=mNodeManager.load(rNodeInfoType, mProjectPathAbs, rName);
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

