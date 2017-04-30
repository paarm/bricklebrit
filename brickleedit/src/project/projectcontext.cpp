#include "projectcontext.h"
#include <QFileInfo>

ProjectContext::ProjectContext() {}

bool ProjectContext::createNewProject(const string& rProjectName, const string& rProjectPathAbs, const string &rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathAbs=rProjectPathAbs;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	mNodeRoot.deleteChildNodes();
	NodeIdGenerator::getInstance().resetNumber();

	NodeProject* rNodeProject=static_cast<NodeProject*>(mNodeRoot.addChildNode(new NodeProject()));
	rNodeProject->setProjectName(rProjectName);

	PropertyList *pl=rNodeProject->getPropertyListListTest();
	pl->addEntry(new PropertyString("Test 1"));
	pl->addEntry(new PropertyString("Test 2"));
	pl->addEntry(new PropertyRectFloat(10.0,20.0,11.0,12.0));

	Node *atlas=rNodeProject->addChildNode(new NodeTextureAtlas());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());
	atlas->addChildNode(new NodeTextureAtlasFrame());

	Node *animation=rNodeProject->addChildNode(new NodeAnimationSet());
	animation->addChildNode(new NodeAnimationSetFrameTexture);
	animation->addChildNode(new NodeAnimationSetFrameTexture);
	animation->addChildNode(new NodeAnimationSetFrameTextureAtlas);
	animation->addChildNode(new NodeAnimationSetFrameTextureAtlas);

	rNodeProject->addChildNode(new NodeSprite());
	rv=rNodeProject->persistSubNode(rProjectPathWithFileAbs);
	return rv;
}

bool ProjectContext::loadProject(const string&rProjectPathWithFileAbs) {
	bool rv=false;
	mProjectPathWithFileAbs=rProjectPathWithFileAbs;
	mNodeRoot.deleteChildNodes();
	NodeIdGenerator::getInstance().resetNumber();

	QFileInfo fi(QString::fromStdString(mProjectPathWithFileAbs));
	if (fi.exists() && fi.isFile()) {
		QString path=fi.absolutePath();
		mProjectPathAbs=path.toStdString();
		NodeParser rNodeParser;
		rNodeParser.parseFile(&mNodeRoot, fi.absoluteFilePath().toStdString());
		if (getNodeProject()) {
			rv=true;
		}
	}
	return rv;
}

string ProjectContext::getProjectPathAbs() {
	return mProjectPathAbs;
}

NodeProject *ProjectContext::getNodeProject() {
	Node *rNodeChild=mNodeRoot.getFirstChildNode();
	if (rNodeChild && rNodeChild->getNodeType()==NodeType::Project) {
		NodeProject *rNodeProject=static_cast<NodeProject*>(rNodeChild);
		return rNodeProject;
	}
	return nullptr;
}

