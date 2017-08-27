#pragma once

#include <vector>
#include <bricklelib.h>
#include <QFileInfo>

template <typename T>
class NodeInfo/* : public NodeInfoBase<T>*/ {
private:
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
		mNode=rOld.mNode;
		mDummy=rOld.mDummy;
		rOld.mNode=nullptr;
	}

	void save(const string& rProjectPathAbs) {
		if (mNode) {
			Node::persistNode(mNode, DirUtil::concatPath(rProjectPathAbs, mNode->getName()));
		}
	}

	void close() {
		if (mNode) {
			delete mNode;
			mNode=nullptr;
		}
	}

	bool createNew(const string& rName) {
		bool rv=true;
		close();
		mNode=new T;
		mNode->setName(rName);
		return rv;
	}

	bool load(const string&rPathWithFileAbs) {
		bool rv=false;
		close();
		//mPathInfo.pathWithFileAbs=rPathWithFileAbs;

		QFileInfo fi(QString::fromStdString(rPathWithFileAbs));
		if (fi.exists() && fi.isFile()) {
			//QString path=fi.absolutePath();
			//mPathInfo.pathAbs=path.toStdString();
			Node *rNode=Node::unpersistNode(fi.absoluteFilePath().toStdString());
			if (rNode) {
				mNode=static_cast<T*>(rNode);
				rv=true;
			}
		}
		return rv;
	}

	T* getNode() {
		return mNode;
	}
};
