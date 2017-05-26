#ifndef MENUBAR_H
#define MENUBAR_H
#include <QMenuBar>

class MenuBar : public QMenuBar
{
private:
	QMenu			*mProjectMenu=nullptr;
	QMenu			*mSceneMenu=nullptr;
public:
	MenuBar(QWidget *parent = Q_NULLPTR);
	void setProjectAvailable(bool isEditable);
};

#endif // MENUBAR_H
