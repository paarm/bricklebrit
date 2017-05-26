#include "toolbar.h"
#include "guicontext.h"

ToolBar::ToolBar(const QString& title, QWidget* parent) : QToolBar (title, parent) {
#if 0
	{
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/project.png"), QSize(), QIcon::Normal, QIcon::Off);
		QToolButton *rToolButtonMenu=new QToolButton(this);
		rToolButtonMenu->setIcon(icon);
		rToolButtonMenu->setText(tr("Project"));
		rToolButtonMenu->setPopupMode(QToolButton::InstantPopup);
		rToolButtonMenu->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
		rToolButtonMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewProject());
		rToolButtonMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenProject());
		addWidget(rToolButtonMenu);
	}
	addSeparator();
	QWidget* spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	addWidget(spacer);
	{
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/scene.png"), QSize(), QIcon::Normal, QIcon::Off);
		QToolButton *rToolButtonMenu=new QToolButton(this);
		rToolButtonMenu->setIcon(icon);
		rToolButtonMenu->setText(tr("Scene"));
		rToolButtonMenu->setPopupMode(QToolButton::InstantPopup);
		rToolButtonMenu->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
		rToolButtonMenu->addAction(GuiContext::getInstance().getMainWindow().getActionNewScene());
		rToolButtonMenu->addAction(GuiContext::getInstance().getMainWindow().getActionOpenScene());

		addWidget(rToolButtonMenu);
	}
#endif


#if 0
	{
		QLabel	*rSceneLabel=new QLabel(tr("Scene"),this);
		addWidget(rSceneLabel);
		QComboBox *rSceneComboBox=new QComboBox(this);
		rSceneComboBox->setMinimumWidth(100);
		addWidget(rSceneComboBox);
	}
	{
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/newscene.png"), QSize(), QIcon::Normal, QIcon::Off);
		QToolButton *rNewScene=new QToolButton(this);
		rNewScene->setIcon(icon);
		rNewScene->setText("New Scnee");
		rNewScene->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
		addWidget(rNewScene);

		//addAction(icon, "New Scene");
		//actions().back()->setIconText("New Scene");
		//QObject::connect(actions().back(), &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onOpenProject);
	}
#endif

}
