#pragma once
#include <QtWidgets>

class ToolBar : public QToolBar
{
private:
	Q_OBJECT
public:
	explicit ToolBar(const QString &title, QWidget *parent = Q_NULLPTR);
};
