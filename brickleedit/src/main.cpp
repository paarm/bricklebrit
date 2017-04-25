#include <QApplication>
#include "gui/guicontext.h"

int main(int argc, char *argv[])
{
	std::setlocale(LC_ALL, "En_US");
	::setlocale(LC_ALL, NULL);
	QApplication a(argc, argv);

	MainWindow rMainWindow;
	GuiContext::getInstance().setMainWindow(&rMainWindow);
	rMainWindow.resize(1280,960);
	rMainWindow.show();

	return a.exec();
}
