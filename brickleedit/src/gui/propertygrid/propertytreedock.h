#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <bricklelib.h>
#include <QLineEdit>
#include <QItemDelegate>

class PropertyInfo {
	Node*	mNode=nullptr;
	string  mPropertyName;
	string  mPropertySubName;
public:
	PropertyInfo() {
	}

	PropertyInfo(Node *rNode, const string& rPropertyName, const string& rPropertySubName) {
		mNode=rNode;
		mPropertyName=rPropertyName;
		mPropertySubName=rPropertySubName;
	}
	const string& getPropertyName() {
		return mPropertyName;
	}
	const string& getPropertySubName() {
		return mPropertySubName;
	}
	Node* getNode() {
		return mNode;
	}
};
Q_DECLARE_METATYPE(PropertyInfo);

class PropertyNumberDelegate : public QItemDelegate
{
public:
	PropertyNumberDelegate(QWidget* parent=Q_NULLPTR);
	~PropertyNumberDelegate() {
	}
protected:
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const {
		QLineEdit *lineEdit = new QLineEdit(parent);

		// Set your validator, such as 'only number between 0 ~ 9 )
		//QIntValidator *validator = new QIntValidator(0, 9, lineEdit);
		//lineEdit->setValidator(validator);
		return lineEdit;
	}
	void setEditorData(QWidget *editor, const QModelIndex &index) const	{
		QString value =index.model()->data(index, Qt::EditRole).toString();
			QLineEdit *line = static_cast<QLineEdit*>(editor);
			line->setText(value);
	}
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QLineEdit *line = static_cast<QLineEdit*>(editor);

		   QIntValidator validator;
		   int pos = 0;
		   QString data = line->text();
		   if(validator.validate(data,pos) == QValidator::Acceptable) {
			   model->setData(index, data);
			   QModelIndex xxx=model->index(index.row(),0, index.parent());
			   QVariant v=model->data(xxx,Qt::UserRole);
			   PropertyInfo rPropertyInfo=v.value<PropertyInfo>();
			   PropertyBase* p=rPropertyInfo.getNode()->getProperty(rPropertyInfo.getPropertyName());
			   if (p) {
				   PropertyType rPropertyType=p->getPropertyType();
				   if (rPropertyType==PropertyType::Bool) {
					   PropertyBool *pp=static_cast<PropertyBool*>(p);
					   pp->value=data=="true"?true:false;
				   } else if (rPropertyType==PropertyType::Int) {
					   PropertyInt *pp=static_cast<PropertyInt*>(p);
					   pp->value=data.toInt();
				   } else if (rPropertyType==PropertyType::String) {
					   PropertyString *pp=static_cast<PropertyString*>(p);
					   pp->value=data.toStdString();
				   } else if (rPropertyType==PropertyType::Float) {
					   PropertyFloat *pp=static_cast<PropertyFloat*>(p);
					   pp->value=data.toFloat();
				   } else if (rPropertyType==PropertyType::RectInt) {
					   PropertyRectInt *pp=static_cast<PropertyRectInt*>(p);
					   if (rPropertyInfo.getPropertySubName()=="X") {
						   pp->value.x=data.toInt();
					   } else if (rPropertyInfo.getPropertySubName()=="Y") {
						   pp->value.y=data.toInt();
					   } else if (rPropertyInfo.getPropertySubName()=="Width") {
						   pp->value.width=data.toInt();
					   } else if (rPropertyInfo.getPropertySubName()=="Height") {
						   pp->value.height=data.toInt();
					   }

				   } else if (rPropertyType==PropertyType::RectFloat) {
					   PropertyRectFloat *pp=static_cast<PropertyRectFloat*>(p);
				   } else if (rPropertyType==PropertyType::PointInt) {
					   PropertyPointInt *pp=static_cast<PropertyPointInt*>(p);
					   if (rPropertyInfo.getPropertySubName()=="X") {
						   pp->value.x=data.toInt();
					   } else if (rPropertyInfo.getPropertySubName()=="Y") {
						   pp->value.y=data.toInt();
					   }

				   } else if (rPropertyType==PropertyType::PointFloat) {
					   PropertyPointFloat *pp=static_cast<PropertyPointFloat*>(p);
				   } else if (rPropertyType==PropertyType::List) {
					   PropertyList *pp=static_cast<PropertyList*>(p);
				   }
			   }
		   }
	}
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		editor->setGeometry(option.rect);
	}
};

namespace Ui {
class PropertyTreeDock;
}

class PropertyTreeDock : public QDockWidget
{
	Q_OBJECT

public:
	explicit PropertyTreeDock(QWidget *parent = 0);
	~PropertyTreeDock();

	void clear();
	void setPropertiesForNode(Node* rNode);
	PropertyInfo getPropertyInfoFromTreeItem(QTreeWidgetItem* r);
	void setPropertyInfoToTreeItem(QTreeWidgetItem* r, PropertyInfo &rPropertyInfo);
private slots:
	void on_propertyTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	Ui::PropertyTreeDock *ui;
	PropertyNumberDelegate *mPropertyNumberDelegate=nullptr;
	void addSubPropertyInt(Node* rNode, const string& propertyName, QTreeWidgetItem* r, int rValue, const string &rPropertySubName);

};



#endif // PROPERTYWIDGET_H
