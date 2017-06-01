#include "menubar.h"
#include "guicontext.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
	mProjectMenu=addMenu(tr("Project"));
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewProject());
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenProject());
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionSaveProject());
	mProjectMenu->addAction(GuiContext::getInstance().getMainWindow().getActionCloseProject());

	//GuiContext::getInstance().getMainWindow().getActionCloseProject()->setEnabled(false);
	//GuiContext::getInstance().getMainWindow().getActionSaveProject()->setEnabled(false);
	//mSceneMenu=addMenu(tr("Scene"));
	//mSceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());
	//mSceneMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenScene());
	//mSceneMenu->setEnabled(false);
}
#if 0
void MenuBar::setProjectAvailable(bool isEditable) {
	//mSceneMenu->setEnabled(isEditable);
	GuiContext::getInstance().getMainWindow().getActionCloseProject()->setEnabled(isEditable);
	GuiContext::getInstance().getMainWindow().getActionSaveProject()->setEnabled(isEditable);
}
#endif
