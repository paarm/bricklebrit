#include "menubar.h"
#include "guicontext.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
	mProjectMenu=addMenu(tr("Project"));
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewProject());
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenProject());

	mSceneMenu=addMenu(tr("Scene"));
	mSceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());
	mSceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenScene());
	mSceneMenu->setEnabled(false);
}

void MenuBar::setProjectAvailable(bool isEditable) {
	mSceneMenu->setEnabled(isEditable);
}
