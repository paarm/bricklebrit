#include "mainwindow.h"
#include "toolbar.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("Brickleedit");
	// GL Widget
	mSceneGlWidget=new SceneGlWidget(this);
	this->setCentralWidget(mSceneGlWidget);
	// Toolbar
	mToolBar=new ToolBar("Tools", this);
	this->addToolBar(mToolBar);
	// Tree Dock
	mSceneTreeDock=new SceneTreeDock(this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mSceneTreeDock);
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
	// Properties Dock
	mElementPropertyDock=new QDockWidget("Properties", this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mElementPropertyDock);
}

MainWindow::~MainWindow()
{
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

