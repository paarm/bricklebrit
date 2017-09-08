#ifndef ANIMATIONFRAMEEDITOR_H
#define ANIMATIONFRAMEEDITOR_H

#include <QDialog>
#include <bricklelib.h>

namespace Ui {
class AnimationFrameEditor;
}

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

private:
	Ui::AnimationFrameEditor *ui;
	Node*	mNode=nullptr;
	void setupNode();
};

#endif // ANIMATIONFRAMEEDITOR_H
