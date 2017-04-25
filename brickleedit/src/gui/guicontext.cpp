#include "guicontext.h"

GuiContext::GuiContext()
{

}

MainWindow& GuiContext::getMainWindow() {
	return *mMainWindow;
}

void GuiContext::setMainWindow(MainWindow* rMainWindow) {
	mMainWindow=rMainWindow;
}

void GuiContext::setWindowTitle(const QString& rProjectName) {
	getMainWindow().setWindowTitle("Brickleedit - "+rProjectName);
}

void GuiContext::onNewProject() {
	getMainWindow().getNewProjectDialog().init();
	getMainWindow().getNewProjectDialog().show();
}
