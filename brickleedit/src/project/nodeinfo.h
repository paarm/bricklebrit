#pragma once

#include <vector>
#include <bricklelib.h>
#include <QFileInfo>

struct PathInfo {
	string			pathAbs="";
	string			pathWithFileAbs="";
	void clear() {
		pathAbs="";
		pathWithFileAbs="";
	}
};
/*
template <typename T>
class NodeInfoBase {
	virtual void	close(bool rPersistBefore)=0;
	virtual void	save()=0;
	virtual bool	createNew(const string& rName, const string& rPathAbs, const string &rPathWithFileAbs)=0;
	virtual bool	load(const string&rPathWithFileAbs)=0;
	virtual string	getPathAbs()=0;
	virtual T*		getNode()=0;
};*/

template <typename T>
class NodeInfo/* : public NodeInfoBase<T>*/ {
private:
	PathInfo	mPathInfo;
	T*			mNode=nullptr;
	int			mDummy;
public:
	NodeInfo(int rDummy) {
		mDummy=rDummy;
	}
	~NodeInfo() {
		close();
	}
	NodeInfo(NodeInfo const&) = delete;
	NodeInfo& operator=(NodeInfo const&)=delete;

	NodeInfo(NodeInfo && rOld) {
		mPathInfo=rOld.mPathInfo;
		mNode=rOld.mNode;
		mDummy=rOld.mDummy;
		rOld.mNode=nullptr;
	}

	void save() {
		if (mNode) {
			Node::persistNode(mNode, mPathInfo.pathWithFileAbs);
		}
	}

	void close() {
		if (mNode) {
			delete mNode;
			mNode=nullptr;
		}
		mPathInfo.clear();
	}

	bool createNew(const string& rName, const string& rPathAbs, const string &rPathWithFileAbs) {
		bool rv=true;
		close();
		mPathInfo.pathAbs=rPathAbs;
		mPathInfo.pathWithFileAbs=rPathWithFileAbs;
		mNode=new T;
		mNode->setName(rName);
		//rv=Node::persistNode(mNode, rPathWithFileAbs);
		return rv;
	}

	bool load(const string&rPathWithFileAbs) {
		bool rv=false;
		close();
		mPathInfo.pathWithFileAbs=rPathWithFileAbs;

		QFileInfo fi(QString::fromStdString(mPathInfo.pathWithFileAbs));
		if (fi.exists() && fi.isFile()) {
			QString path=fi.absolutePath();
			mPathInfo.pathAbs=path.toStdString();
			Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
			if (rNode) {
				mNode=static_cast<T*>(rNode);
				rv=true;
			}
		}
		return rv;
	}

	string getPathAbs() {
		return mPathInfo.pathAbs;
	}

	string getPathWithFileAbs() {
		return mPathInfo.pathWithFileAbs;
	}

	T* getNode() {
		return mNode;
	}
};
