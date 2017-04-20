#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("Brickleedit");
	// GL Widget
	mSceneGlWidget=new SceneGlWidget(this);
	this->setCentralWidget(mSceneGlWidget);
	// Toolbar
	mToolBar=new QToolBar("Tools", this);
	this->addToolBar(mToolBar);
	// Tree Dock
	mTreeDock=new QDockWidget("Tree", this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mTreeDock);
	QWidget	*panel=new QWidget(mTreeDock);
	QVBoxLayout *vTreeLayout = new QVBoxLayout();
	QHBoxLayout *treeLayout = new QHBoxLayout();
	QPushButton *buttonTest=new QPushButton("Test", mTreeDock);
	buttonTest->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	treeLayout->addWidget(buttonTest);
	treeLayout->addWidget(new QPushButton("Test 2", mTreeDock));
	treeLayout->addSpacerItem(new QSpacerItem(1,1));
	vTreeLayout->addLayout(treeLayout);
	vTreeLayout->addWidget(new QTreeWidget(mTreeDock));

	panel->setLayout(vTreeLayout);
	mTreeDock->setWidget(panel);


	// Properties Dock
	mPropertiesDock=new QDockWidget("Properties", this);
	this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, mPropertiesDock);

}

MainWindow::~MainWindow()
{
}
