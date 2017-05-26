#include "propertytreedock.h"
#include "ui_propertytreedock.h"


PropertyNumberDelegate::PropertyNumberDelegate(QWidget* parent) : QItemDelegate(parent) {

}

PropertyTreeDock::PropertyTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::PropertyTreeDock)
{
	mPropertyNumberDelegate = new PropertyNumberDelegate(this);
	ui->setupUi(this);
	ui->propertyTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

PropertyTreeDock::~PropertyTreeDock()
{
	delete ui;
}

void PropertyTreeDock::clear() {
	ui->propertyTree->clear();
}

PropertyInfo PropertyTreeDock::getPropertyInfoFromTreeItem(QTreeWidgetItem* r) {
	PropertyInfo rv;
	if (r) {
		QVariant v=r->data(0,Qt::UserRole);
		rv=v.value<PropertyInfo>();
	}
	return rv;
}

void PropertyTreeDock::setPropertyInfoToTreeItem(QTreeWidgetItem* r, PropertyInfo &rPropertyInfo) {
	if (r) {
		QVariant v;
		v.setValue(rPropertyInfo);
		r->setData(0, Qt::UserRole, v);
	}
}

void PropertyTreeDock::addSubPropertyInt(Node* rNode, const string& propertyName, QTreeWidgetItem* r, int rValue, const string &rPropertySubName) {
	QTreeWidgetItem* rr=nullptr;
	PropertyInfo rPropertyInfo(rNode, propertyName, rPropertySubName);
	rr=new QTreeWidgetItem(r);
	setPropertyInfoToTreeItem(rr, rPropertyInfo);
	rr->setText(0,QString::fromStdString(rPropertySubName));
	rr->setText(1,QString::fromStdString(std::to_string(rValue)));
	rr->setFlags(rr->flags()|Qt::ItemIsEditable);
	r->addChild(rr);
	//r->setExpanded(true);
}


void PropertyTreeDock::setPropertiesForNode(Node* rNode) {
	clear();
	if (rNode) {
		unsigned long count=rNode->getPropertyCount();
		for (unsigned long i=0;i<count;i++) {
			PropertyBase* p=rNode->getPropertyFromIndex(i);
			if (p) {
				QTreeWidgetItem* r=nullptr;
				PropertyInfo rPropertyInfo(rNode, p->getPropertyName(), "");
				r=new QTreeWidgetItem(ui->propertyTree);
				setPropertyInfoToTreeItem(r, rPropertyInfo);
				PropertyType rPropertyType=p->getPropertyType();
				if (rPropertyType==PropertyType::Bool) {
					PropertyBool *pp=static_cast<PropertyBool*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,pp->value==true?"true":"false");
				} else if (rPropertyType==PropertyType::Int) {
					PropertyInt *pp=static_cast<PropertyInt*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(std::to_string(pp->value)));
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::String) {
					PropertyString *pp=static_cast<PropertyString*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(pp->value));
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::Float) {
					PropertyFloat *pp=static_cast<PropertyFloat*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(std::to_string(pp->value)));
				} else if (rPropertyType==PropertyType::RectInt) {
					PropertyRectInt *pp=static_cast<PropertyRectInt*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(
								   "X="+std::to_string(pp->value.x)+
								   " | Y="+std::to_string(pp->value.y)+
								   " | W="+std::to_string(pp->value.width)+
								   " | H="+std::to_string(pp->value.height)));

					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.x, "X");
					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.y, "Y");
					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.width, "Width");
					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.height, "Height");
				} else if (rPropertyType==PropertyType::RectFloat) {
					PropertyRectFloat *pp=static_cast<PropertyRectFloat*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(
								   "X="+std::to_string(pp->value.x)+
								   " | Y="+std::to_string(pp->value.y)+
								   " | W="+std::to_string(pp->value.width)+
								   " | H="+std::to_string(pp->value.height)));
				} else if (rPropertyType==PropertyType::PointInt) {
					PropertyPointInt *pp=static_cast<PropertyPointInt*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(
								   "X="+std::to_string(pp->value.x)+
								   " | Y="+std::to_string(pp->value.y)));
					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.x, "X");
					addSubPropertyInt(rNode, pp->getPropertyName(), r, pp->value.y, "Y");
				} else if (rPropertyType==PropertyType::PointFloat) {
					PropertyPointFloat *pp=static_cast<PropertyPointFloat*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(
								   "X="+std::to_string(pp->value.x)+
								   " | Y="+std::to_string(pp->value.y)));
				} else if (rPropertyType==PropertyType::List) {
					PropertyList *pp=static_cast<PropertyList*>(p);
				}
				ui->propertyTree->addTopLevelItem(r);
			}
		}
	}
}

void PropertyTreeDock::on_propertyTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (column==1) {
		ui->propertyTree->setItemDelegateForColumn(column, mPropertyNumberDelegate);
		ui->propertyTree->editItem(item, column);
	}
}
