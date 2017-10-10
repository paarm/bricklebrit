#ifndef GRIDEDITOR_H
#define GRIDEDITOR_H

#include <QDialog>

namespace Ui {
class GridEditor;
}

class GridEditor : public QDialog
{
	Q_OBJECT

public:
	explicit GridEditor(int gridX, int gridY, int offsetX, int offsetY, QWidget *parent = 0);
	~GridEditor();

private slots:
	void on_cancelButton_clicked();

	void on_okButton_clicked();

	void on_GridEditor_rejected();

private:
	Ui::GridEditor *ui;
};

#endif // GRIDEDITOR_H
