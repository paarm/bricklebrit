#include "toolbar.h"
#include "guicontext.h"

ToolBar::ToolBar(const QString& title, QWidget* parent) : QToolBar (title, parent) {
	{
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/newproject.png"), QSize(), QIcon::Normal, QIcon::Off);
		addAction(icon, "New Project");
		QObject::connect(actions().back(), &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewProject);
	}
	{
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/openproject.png"), QSize(), QIcon::Normal, QIcon::Off);
		addAction(icon, "Open Project");
		QObject::connect(actions().back(), &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onOpenProject);
	}
}
