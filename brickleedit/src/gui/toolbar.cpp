#include "toolbar.h"
#include "guicontext.h"

ToolBar::ToolBar(const QString& title, QWidget* parent) : QToolBar (title, parent) {
	addAction(QIcon("data/icons/newproject.png"), "New Project");
	QObject::connect(actions().back(), &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewProject);
}
