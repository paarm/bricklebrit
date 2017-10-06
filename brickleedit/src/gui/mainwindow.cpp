#include "mainwindow.h"
#include "toolbar.h"
#include "guicontext.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	GuiContext::getInstance().setMainWindow(this);

	setWindowTitle("Brickleedit");

	mActionNewProject = new QAction(QIcon(":/icons/new.png"), tr("New Project..."), this);
	//mActionNewProject->setShortcuts(QKeySequence::Open);
	mActionNewProject->setStatusTip(tr("Create a new Project"));
	QObject::connect(mActionNewProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewProjectClicked);

	mActionOpenProject = new QAction(QIcon(":/icons/open.png"), tr("Open Project..."), this);
	//mActionOpenProject->setShortcuts(QKeySequence::Open);
	mActionOpenProject->setStatusTip(tr("Open a existing Project"));
	QObject::connect(mActionOpenProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onOpenProjectClicked);

	mActionSaveProject = new QAction(/*QIcon(":/icons/open.png"),*/ tr("Save Project..."), this);
    mActionSaveProject->setShortcuts(QKeySequence::Save);
	mActionSaveProject->setStatusTip(tr("Save the Project"));
	QObject::connect(mActionSaveProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onSaveProjectClicked);

	mActionCloseProject = new QAction(/*QIcon(":/icons/open.png"), */tr("Close Project"), this);
	//mActionOpenProject->setShortcuts(QKeySequence::Open);
	mActionCloseProject->setStatusTip(tr("Close a existing Project"));
	QObject::connect(mActionCloseProject, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onCloseProjectClicked);

	mActionNewScene = new QAction(QIcon(":/icons/new.png"), tr("&New Scene..."), this);
	mActionNewScene->setShortcuts(QKeySequence::New);
	mActionNewScene->setStatusTip(tr("Create a new Scene"));
	QObject::connect(mActionNewScene, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewSceneClicked);

	mActionNewResource = new QAction(QIcon(":/icons/new.png"), tr("&New Resource..."), this);
	mActionNewResource->setShortcuts(QKeySequence::New);
	mActionNewResource->setStatusTip(tr("Create a new Resource"));
	QObject::connect(mActionNewResource, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewResourceClicked);

//	QAction			*mActionOpenProject=nullptr;
//	QAction			*mActionNewScene=nullptr;
//	QAction			*mActionOpenScene=nullptr;




	// GL Widget
	mSceneGlWidget=new SceneGlWidget(this);
	this->setCentralWidget(mSceneGlWidget);
	// Menue Bar
	mMenuBar=new MenuBar(this);
	this->setMenuBar(mMenuBar);

	// Toolbar
	mToolBar=new ToolBar("Tools", this);
	this->addToolBar(mToolBar);
	// Tree Dock
	mSceneTreeDock=new SceneTreeDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mSceneTreeDock);
	// Brush Dock
	mBrushDock=new BrushDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mBrushDock);
	// Scene Properties Dock
	mScenePropertyTreeDock=new PropertyTreeDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mScenePropertyTreeDock);

	setProjectAvailable(false);
	setProjectRequireSave(false);
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
QAction*	MainWindow::getActionCloseProject() {
	return mActionCloseProject;
}
QAction*	MainWindow::getActionSaveProject() {
	return mActionSaveProject;
}
QAction*	MainWindow::getActionNewScene() {
	return mActionNewScene;
}

QAction*	MainWindow::getActionNewResource() {
	return mActionNewResource;
}

ToolBar& MainWindow::getToolBar() {
	return *mToolBar;
}

MenuBar& MainWindow::getMenuBar() {
	return *mMenuBar;
}

NewProjectDialog& MainWindow::getNewProjectDialog() {
	if (mNewProjectDialog==nullptr) {
		mNewProjectDialog=new NewProjectDialog(this);
	}
	return *mNewProjectDialog;
}

NewSceneDialog& MainWindow::getNewSceneDialog() {
	if (mNewSceneDialog==nullptr) {
		mNewSceneDialog=new NewSceneDialog(false, this);
	}
	return *mNewSceneDialog;
}

NewSceneDialog& MainWindow::getNewResourceDialog() {
	if (mNewResourceDialog==nullptr) {
		mNewResourceDialog=new NewSceneDialog(true, this);
	}
	return *mNewResourceDialog;
}

void MainWindow::setProjectAvailable(bool isAvailable) {
	getActionCloseProject()->setEnabled(isAvailable);
	getActionNewProject()->setEnabled(!isAvailable);
	getActionOpenProject()->setEnabled(!isAvailable);
}
void MainWindow::setProjectRequireSave(bool rRequireSave) {
	getActionSaveProject()->setEnabled(rRequireSave);
}


SceneTreeDock& MainWindow::getSceneTreeDock() {
	return *mSceneTreeDock;
}

PropertyTreeDock& MainWindow::getPropertyTreeDock() {
	return *mScenePropertyTreeDock;
}

BrushDock& MainWindow::getBrushDock() {
	return *mBrushDock;
}

SceneGlWidget& MainWindow::getSceneGlWidget() {
	return *mSceneGlWidget;
}

