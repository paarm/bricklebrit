#include "mainwindow.h"
#include "toolbar.h"
#include "guicontext.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	GuiContext::getInstance().setMainWindow(this);

	setWindowTitle("Brickleedit");

	mActionNewProject = new QAction(QIcon(":/icons/new.png"), tr("&New Project..."), this);
	//mActionNewProject->setShortcuts(QKeySequence::Open);
	mActionNewProject->setStatusTip(tr("Create a new Project"));
	QObject::connect(mActionNewProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewProjectClicked);

	mActionOpenProject = new QAction(QIcon(":/icons/open.png"), tr("&Open Project..."), this);
	//mActionOpenProject->setShortcuts(QKeySequence::Open);
	mActionOpenProject->setStatusTip(tr("Open a existing Project"));
	QObject::connect(mActionOpenProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onOpenProjectClicked);

	mActionNewScene = new QAction(QIcon(":/icons/new.png"), tr("&New Scene..."), this);
	mActionNewScene->setShortcuts(QKeySequence::New);
	mActionNewScene->setStatusTip(tr("Create a new Scene"));
	QObject::connect(mActionNewScene, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewSceneClicked);

	mActionOpenScene = new QAction(QIcon(":/icons/open.png"), tr("&Open Scene..."), this);
	mActionOpenScene->setShortcuts(QKeySequence::Open);
	mActionOpenScene->setStatusTip(tr("Open a existing Scene"));
	QObject::connect(mActionOpenScene, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onOpenSceneClicked);



//	QAction			*mActionOpenProject=nullptr;
//	QAction			*mActionNewScene=nullptr;
//	QAction			*mActionOpenScene=nullptr;




	// GL Widget
	mSceneGlWidget=new SceneGlWidget(this);
	this->setCentralWidget(mSceneGlWidget);
	// Toolbar
	mToolBar=new ToolBar("Tools", this);
	this->addToolBar(mToolBar);
	// Tree Dock
	mSceneTreeDock=new SceneTreeDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mSceneTreeDock);
	// Scene Properties Dock
	mScenePropertyTreeDock=new PropertyTreeDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mScenePropertyTreeDock);
#if 0
	QWidget	*panel=new QWidget(mSceneTreeDock);
	QVBoxLayout *vTreeLayout = new QVBoxLayout();
	QHBoxLayout *treeLayout = new QHBoxLayout();
	QPushButton *buttonTest=new QPushButton("Test", mSceneTreeDock);
	buttonTest->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	treeLayout->addWidget(buttonTest);
	treeLayout->addWidget(new QPushButton("Test 2", mSceneTreeDock));
	treeLayout->addSpacerItem(new QSpacerItem(1,1));
	vTreeLayout->addLayout(treeLayout);
	vTreeLayout->addWidget(new QTreeWidget(mSceneTreeDock));

	panel->setLayout(vTreeLayout);
	mSceneTreeDock->setWidget(panel);
#endif
}

MainWindow::~MainWindow()
{
}

QAction*	MainWindow::getActionNewProject() {
	return mActionNewProject;
}
QAction*	MainWindow::getActionOpenProject() {
	return mActionOpenProject;
}
QAction*	MainWindow::getActionNewScene() {
	return mActionNewScene;
}
QAction*	MainWindow::getActionOpenScene() {
	return mActionOpenScene;
}




ToolBar& MainWindow::getToolBar() {
	return *mToolBar;
}

NewProjectDialog& MainWindow::getNewProjectDialog() {
	if (mNewProjectDialog==nullptr) {
		mNewProjectDialog=new NewProjectDialog(this);
	}
	return *mNewProjectDialog;
}

NewSceneDialog& MainWindow::getNewSceneDialog() {
	if (mNewSceneDialog==nullptr) {
		mNewSceneDialog=new NewSceneDialog(this);
	}
	return *mNewSceneDialog;
}

SceneTreeDock& MainWindow::getSceneTreeDock() {
	return *mSceneTreeDock;
}

PropertyTreeDock& MainWindow::getPropertyTreeDock() {
	return *mScenePropertyTreeDock;
}


