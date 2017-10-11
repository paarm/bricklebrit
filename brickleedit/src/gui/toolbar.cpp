#include "toolbar.h"
#include "guicontext.h"

ToolBar::ToolBar(const QString& title, QWidget* parent) : QToolBar (title, parent) {
	mZoomLevel=new QLineEdit(this);
	mZoomLevel->setEnabled(false);
    mZoomLevel->setMaximumWidth(120);
	updateZoomLevel(0);
	mActionGroupTools = new QActionGroup(this);
	mActionGroupTools->addAction(GuiContext::getInstance().getMainWindow().getActionToolSelection());
	mActionGroupTools->addAction(GuiContext::getInstance().getMainWindow().getActionToolBrush());
	mActionGroupTools->setExclusive(true);
	addAction(GuiContext::getInstance().getMainWindow().getActionToolSelection());
	addAction(GuiContext::getInstance().getMainWindow().getActionToolBrush());
	addSeparator();
	addAction(GuiContext::getInstance().getMainWindow().getActionPickAsBrush());
    addAction(GuiContext::getInstance().getMainWindow().getActionEraseSelected());
    addSeparator();
	addAction(GuiContext::getInstance().getMainWindow().getActionGrid());
	addAction(GuiContext::getInstance().getMainWindow().getActionGridEdit());
	addSeparator();
	addWidget(mZoomLevel);
}

void ToolBar::updateZoomLevel(int rZoomLevel) {
	mZoomLevel->setText(QString::fromStdString("Zoom: "+to_string(rZoomLevel)));
}
