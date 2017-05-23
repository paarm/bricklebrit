#include "propertytreedock.h"
#include "ui_propertytreedock.h"

PropertyTreeDock::PropertyTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::PropertyTreeDock)
{
	ui->setupUi(this);
}

PropertyTreeDock::~PropertyTreeDock()
{
	delete ui;
}

void PropertyTreeDock::clear() {
	ui->propertyTree->clear();
}

void PropertyTreeDock::setPropertiesForNode(Node* rNode) {
	clear();
	if (rNode) {
		unsigned long count=rNode->getPropertyCount();
		for (unsigned long i=0;i<count;i++) {
			PropertyBase* p=rNode->getPropertyFromIndex(i);
			if (p) {
				QTreeWidgetItem* r=nullptr;
				r=new QTreeWidgetItem(ui->propertyTree);
				PropertyType rPropertyType=p->getPropertyType();
				if (rPropertyType==PropertyType::Bool) {
					PropertyBool *pp=static_cast<PropertyBool*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,pp->value==true?"true":"false");
				} else if (rPropertyType==PropertyType::Int) {
					PropertyInt *pp=static_cast<PropertyInt*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(std::to_string(pp->value)));
				} else if (rPropertyType==PropertyType::String) {
					PropertyString *pp=static_cast<PropertyString*>(p);
					r->setText(0,QString::fromStdString(pp->getPropertyName()));
					r->setText(1,QString::fromStdString(pp->value));
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
