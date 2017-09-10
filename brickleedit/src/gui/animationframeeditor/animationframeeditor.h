#ifndef ANIMATIONFRAMEEDITOR_H
#define ANIMATIONFRAMEEDITOR_H

#include <QDialog>
#include <bricklelib.h>
#include <QTreeWidgetItem>

namespace Ui {
class AnimationFrameEditor;
}

struct AnimationFrameEntry {
	string	resourcefile;
	int		textureid=0;
	string	frame;
	AnimationFrameEntry(const string&resourcefile, int textureid, const string &frame) {
		this->resourcefile=resourcefile;
		this->textureid=textureid;
		this->frame=frame;
	}
	bool isEqual(const string&resourcefile, int textureid, const string &frame) {
		if (this->resourcefile==resourcefile && this->textureid==textureid && this->frame==frame) {
			return true;
		}
		return false;
	}
};

class AnimationFrameEditor : public QDialog
{
	Q_OBJECT

public:
	AnimationFrameEditor(Node *rNode, QWidget *parent = 0);
	~AnimationFrameEditor();

private slots:
	void on_okButton_clicked();

	void on_cancelButton_clicked();

	void on_AnimationFrameEditor_rejected();

	void on_resourceNameOpm_currentIndexChanged(int index);

	void on_addFrame_clicked();

	void on_removeFrame_clicked();

	void on_treeWidgetTextures_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	Ui::AnimationFrameEditor *ui;
	Node*	mNode=nullptr;
	vector<AnimationFrameEntry> mAnimationFrameEntryList;
	void setupNode();
	void setupCurrentResource(const string &rName);
	void buildAnimationFrameView();
	void addSelectedFrames(bool setNextSelection);

};

#endif // ANIMATIONFRAMEEDITOR_H
