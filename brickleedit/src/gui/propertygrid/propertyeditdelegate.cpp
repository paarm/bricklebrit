#include "propertyeditdelegate.h"

PropertyEditDelegate::PropertyEditDelegate(IProperty* rIProperty, QWidget* parent) : QItemDelegate(parent) {
	mIProperty=rIProperty;
}

PropertyEditDelegate::~PropertyEditDelegate() {
}

QWidget* PropertyEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	QModelIndex xxx=index.model()->index(index.row(),0, index.parent());
	QVariant v=index.model()->data(xxx,Qt::UserRole);
	PropertyInfo rPropertyInfo=v.value<PropertyInfo>();

	if (rPropertyInfo.getPropertyBase() && rPropertyInfo.getPropertyBase()->getPropertyType()==PropertyType::Bool) {
		QComboBox *rComboBox=new QComboBox(parent);
		rComboBox->insertItem(0, "false");
		rComboBox->insertItem(1, "true");
		return rComboBox;
	} else {
		return new QLineEdit(parent);
	}

	// Set your validator, such as 'only number between 0 ~ 9 )
	//QIntValidator *validator = new QIntValidator(0, 9, lineEdit);
	//lineEdit->setValidator(validator);
	//return lineEdit;
}
void PropertyEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const	{
	QString value =index.model()->data(index, Qt::EditRole).toString();

	QModelIndex xxx=index.model()->index(index.row(),0, index.parent());
	QVariant v=index.model()->data(xxx,Qt::UserRole);
	PropertyInfo rPropertyInfo=v.value<PropertyInfo>();
	if (rPropertyInfo.getPropertyBase() && rPropertyInfo.getPropertyBase()->getPropertyType()==PropertyType::Bool) {
		QComboBox *line = static_cast<QComboBox*>(editor);
		if (line) {
			int index = line->findText(value);
			if (index>=0) {
				line->setCurrentIndex(index);
				if (value=="true") {
					line->setCurrentIndex(1);
				} else {
					line->setCurrentIndex(0);
				}
			} else {
				line->setCurrentIndex(0);
			}
		}
	} else {
		QLineEdit *line = static_cast<QLineEdit*>(editor);
		line->setText(value);
	}
}

void PropertyEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QModelIndex xxx=model->index(index.row(),0, index.parent());
	QVariant v=model->data(xxx,Qt::UserRole);
	if (mIProperty) {
		QString data="";
		PropertyInfo rPropertyInfo=v.value<PropertyInfo>();
		if (rPropertyInfo.getPropertyBase() && rPropertyInfo.getPropertyBase()->getPropertyType()==PropertyType::Bool) {
			QComboBox *line = static_cast<QComboBox*>(editor);
			if (line->currentIndex()==1) {
				data="true";
			} else {
				data="false";
			}
		} else {
			QLineEdit *line = static_cast<QLineEdit*>(editor);
			data = line->text();
		}

		if (mIProperty->onPropertyChange(&rPropertyInfo, data)) {
			model->setData(index, data);
		}
	}
}

void PropertyEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	editor->setGeometry(option.rect);
}
