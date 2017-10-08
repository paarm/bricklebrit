#include <QApplication>
#include "gui/guicontext.h"

int main(int argc, char *argv[])
{
	std::setlocale(LC_ALL, "En_US");
	::setlocale(LC_ALL, NULL);
	QApplication a(argc, argv);


    QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		a.setStyleSheet(ts.readAll());
	}

	MainWindow rMainWindow;
	rMainWindow.resize(1280,960);
	rMainWindow.show();

	return a.exec();
}
