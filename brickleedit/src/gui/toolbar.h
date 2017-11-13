#pragma once
//#include <QtWidgets>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QToolBar>
struct BrushFields {
	QLineEdit *mParentNode=nullptr;
	QLineEdit *mBrushNode=nullptr;
	QSpinBox *mSizeX=nullptr;
	QSpinBox *mSizeY=nullptr;
	QSpinBox *mScaleX=nullptr;
	QSpinBox *mScaleY=nullptr;
	QSpinBox *mRotate=nullptr;
	QCheckBox *mSnapActive=nullptr;
	QSpinBox *mSnapOffxetX=nullptr;
	QSpinBox *mSnapOffsetY=nullptr;
	QSpinBox *mSnapX=nullptr;
	QSpinBox *mSnapY=nullptr;
};

class ToolBar : public QToolBar
{
private:
	Q_OBJECT
	QLineEdit *mZoomLevel;
	BrushFields mBrushFields;
	QActionGroup	*mActionGroupTools=nullptr;

public:
	explicit ToolBar(const QString &title, QWidget *parent = Q_NULLPTR);
	void updateZoomLevel(int rZoomLevel);
};
