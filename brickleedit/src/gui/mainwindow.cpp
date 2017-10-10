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

	mActionNewScene = new QAction(QIcon(":/icons/new.png"), tr("New Scene..."), this);
	mActionNewScene->setShortcuts(QKeySequence::New);
	mActionNewScene->setStatusTip(tr("Create a new Scene"));
	QObject::connect(mActionNewScene, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewSceneClicked);

	mActionNewResource = new QAction(QIcon(":/icons/new.png"), tr("New Resource..."), this);
	mActionNewResource->setShortcuts(QKeySequence::New);
	mActionNewResource->setStatusTip(tr("Create a new Resource"));
	QObject::connect(mActionNewResource, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onNewResourceClicked);

	mActionToolSelection = new QAction(QIcon(":/icons/selection.png"), tr("Selection Tool"), this);
	mActionToolSelection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
	mActionToolSelection->setStatusTip(tr("Selection Tool"));
	QObject::connect(mActionToolSelection, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onToolSelectionActivated);
	mActionToolSelection->setCheckable(true);
	//mActionToolSelection->setChecked(true);

	mActionToolBrush = new QAction(QIcon(":/icons/brush.png"), tr("Brush Tool"), this);
	mActionToolBrush->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
	mActionToolBrush->setStatusTip(tr("Brush Tool"));
	QObject::connect(mActionToolBrush, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onToolBrushActivated);
	mActionToolBrush->setCheckable(true);

	mActionGrid = new QAction(QIcon(":/icons/grid.png"), tr("Snap to grid on/off"), this);
	mActionGrid->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	QObject::connect(mActionGrid, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onGridCheckedChanged);
	mActionGrid->setCheckable(true);

	mActionGridEdit = new QAction("Grid Settings..."/*QIcon(":/icons/grid.png")**/, this);
	//mActionGridEdit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	QObject::connect(mActionGridEdit, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onGridEdit);

	mActionPickAsBrush = new QAction(QIcon(":/icons/pickAsBrush.png"), tr("Pick selected Node as Brush"), this);
	mActionPickAsBrush->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	QObject::connect(mActionPickAsBrush, &QAction::triggered, &GuiContext::getInstance(), &GuiContext::onPickAsBrush);

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
	setSceneAvailable(false);
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

QAction*	MainWindow::getActionToolSelection() {
	return mActionToolSelection;
}

QAction*	MainWindow::getActionToolBrush() {
	return mActionToolBrush;
}

QAction*	MainWindow::getActionGrid() {
	return mActionGrid;
}

QAction*	MainWindow::getActionGridEdit() {
	return mActionGridEdit;
}

QAction*	MainWindow::getActionPickAsBrush() {
	return mActionPickAsBrush;
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
	getActionGrid()->setEnabled(isAvailable);
	getActionGridEdit()->setEnabled(isAvailable);
	if (!isAvailable) {
		getActionGrid()->setChecked(false);
	} else {
		getActionGrid()->setChecked(ProjectContext::getInstance().getNodeProject()->getGridActive());
	}
}

void MainWindow::setSceneAvailable(bool isAvailable) {
	getActionToolSelection()->setEnabled(isAvailable);
	getActionToolBrush()->setEnabled(isAvailable);
	getActionPickAsBrush()->setEnabled(isAvailable);
	if (isAvailable) {
		if (!getActionToolSelection()->isChecked() && !getActionToolBrush()->isChecked()) {
			getActionToolSelection()->setChecked(true);
		}
	}
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

