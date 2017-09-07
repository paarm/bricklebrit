#ifndef TEXTUREFRAMEEDITOR_H
#define TEXTUREFRAMEEDITOR_H

#include <QDialog>
#include <bricklelib.h>

namespace Ui {
class TextureFrameEditor;
}

struct TextureFrameEntry {
	TextureFrameEntry(const string&rName, int x, int y, int w, int h) {
		name=rName;
		this->x=x;
		this->y=y;
		this->w=w;
		this->h=h;
	}
	bool equal(int x, int y, int w, int h) {
		bool rv=false;
		if (this->x==x && this->y==y && this->w==w && this->h==h) {
			rv=true;
		}
		return rv;
	}
	string name;
	int x;
	int y;
	int w;
	int h;
};

class TextureFrameEditor : public QDialog
{
	Q_OBJECT

public:
	TextureFrameEditor(Node *rNode, QWidget *parent = 0);
	~TextureFrameEditor();

private slots:
	void on_cancelButton_clicked();

	void on_okButton_clicked();

	void on_TextureFrameEditor_rejected();

	void on_addFrames_clicked();

	void on_addSingleFrame_clicked();

	void on_removeFrame_clicked();

private:
	Ui::TextureFrameEditor *ui;
	vector<TextureFrameEntry> mTextureFrameEntryList;
	Node*	mNode=nullptr;
	QPixmap  mImage;
	QPixmap  mImageScaled;
	int		 mImageWidth;
	int		 mImageHeight;
	void setupNode();
	void createFrame(const string&rName, int x, int y, int w, int h);
	void updateFrameView();

};


#endif // TEXTUREFRAMEEDITOR_H
