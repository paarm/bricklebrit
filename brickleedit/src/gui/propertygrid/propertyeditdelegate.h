#pragma once
#include <QItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include "propertyinfo.h"
#include "iproperty.h"

class PropertyEditDelegate : public QItemDelegate
{
private:
	IProperty* mIProperty=nullptr;
public:
	PropertyEditDelegate(IProperty* rIProperty, QWidget* parent=Q_NULLPTR);
	~PropertyEditDelegate();
protected:
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
