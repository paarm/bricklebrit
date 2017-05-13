#include "projectcontext.h"

ProjectContext::ProjectContext() {
}

ProjectContext::~ProjectContext() {
}

void ProjectContext::closeProject(bool rPersistBefore) {
	mCurrentProjectInfo.closeProject(rPersistBefore);
}

bool ProjectContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs) {
	return mCurrentProjectInfo.createNewProject(rProjectName, rProjectPathAbs, rProjectPathWithFileAbs);
}

bool ProjectContext::loadProject(const string&rProjectPathWithFileAbs) {
	return mCurrentProjectInfo.loadProject(rProjectPathWithFileAbs);
}

string ProjectContext::getProjectPathAbs() {
	return mCurrentProjectInfo.getProjectPathAbs();
}

NodeProject *ProjectContext::getNodeProject() {
	return mCurrentProjectInfo.getNodeProject();
}

string ProjectContext::getCurrentScenePathAbs() {
	return mCurrentProjectInfo.getCurrentSceneInfo().getScenePathAbs();
}

NodeScene *ProjectContext::getNodeCurrentScene() {
	return mCurrentProjectInfo.getCurrentSceneInfo().getNodeScene();
}

bool ProjectContext::isCurrentSceneAvailable() {
	return mCurrentProjectInfo.getCurrentSceneInfo().isSceneAvailable();
}

void ProjectContext::closeCurrentScene(bool rPersistBefore) {
	mCurrentProjectInfo.getCurrentSceneInfo().closeScene(rPersistBefore);
}

bool ProjectContext::createNewScene(const string& rSceneName, const string& rScenePathAbs, const string &rScenePathWithFileAbs) {
	return mCurrentProjectInfo.getCurrentSceneInfo().createNewScene(rSceneName, rScenePathAbs, rScenePathWithFileAbs);
}

bool ProjectContext::loadCurrentScene(const string&rScenePathWithFileAbs) {
	return mCurrentProjectInfo.getCurrentSceneInfo().loadScene(rScenePathWithFileAbs);
}


