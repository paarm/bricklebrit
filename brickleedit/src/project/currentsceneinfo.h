#pragma once
#include <vector>
#include <bricklelib.h>
#include <QFileInfo>


struct PathInfo {
	string			pathAbs;
	string			pathWithFileAbs;
	void clear() {
		pathAbs="";
		pathWithFileAbs="";
	}
};

class CurrentSceneInfo {
private:
	PathInfo				mPathInfo;
	NodeScene				*mNodeScene=nullptr;
	vector<NodeScene*>		mNodeSceneRefMap;
public:
	virtual ~CurrentSceneInfo();
	void	closeScene(bool rPersistBefore);
	bool	createNewScene(const string& rSceneName, const string& rScenePathAbs, const string &rScenePathWithFileAbs);
	bool	loadScene(const string&rScenePathWithFileAbs);
	bool	isSceneAvailable();

	string	getScenePathAbs();
	NodeScene *getNodeScene();
};

