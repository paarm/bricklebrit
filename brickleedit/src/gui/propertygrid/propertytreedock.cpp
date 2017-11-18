#include "propertytreedock.h"
#include "ui_propertytreedock.h"
#include "../guicontext.h"

PropertyTreeDock::PropertyTreeDock(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::PropertyTreeDock)
{
	mPropertyNumberDelegate = new PropertyEditDelegate(this, this);
	ui->setupUi(this);
	ui->propertyTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

PropertyTreeDock::~PropertyTreeDock()
{
	delete ui;
}

void PropertyTreeDock::clear() {
	ui->propertyTree->clear();
	mNode=nullptr;
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

bool PropertyTreeDock::validatePropertyInt(QString data) {
	QIntValidator validator;
	int				pos=0;
	return validator.validate(data,pos) == QValidator::Acceptable;
}

bool PropertyTreeDock::validatePropertyFloat(QString data) {
	QDoubleValidator validator;
	int				pos=0;
	return validator.validate(data,pos) == QValidator::Acceptable;
}

bool PropertyTreeDock::onPropertyChange(PropertyInfo* rPropertyInfo, QString data) {
	bool rv=false;
	if(rPropertyInfo && rPropertyInfo->getPropertyBase()) {
		PropertyBase* p=rPropertyInfo->getPropertyBase(); //rPropertyInfo->getNode()->getProperty(rPropertyInfo->getPropertyName());
		if (p) {
			PropertyType rPropertyType=p->getPropertyType();
			if (rPropertyType==PropertyType::Bool) {
				PropertyBool *pp=static_cast<PropertyBool*>(p);
				pp->value=data=="true"?true:false;
				rv=true;
			} else if (rPropertyType==PropertyType::Int) {
				rv=validatePropertyInt(data);
				if (rv) {
					PropertyInt *pp=static_cast<PropertyInt*>(p);
					pp->value=data.toInt();
				}
			} else if (rPropertyType==PropertyType::String) {
				PropertyString *pp=static_cast<PropertyString*>(p);
				pp->value=data.toStdString();
				rv=true;
			} else if (rPropertyType==PropertyType::Float) {
				PropertyFloat *pp=static_cast<PropertyFloat*>(p);
				pp->value=data.toFloat();
				rv=true;
			} else if (rPropertyType==PropertyType::Ref) {
				PropertyRef *pp=static_cast<PropertyRef*>(p);
				if (rPropertyInfo->getPropertySubName()=="File") {
					pp->value.reffile=data.toStdString();
					rv=true;
				} else if (rPropertyInfo->getPropertySubName()=="Id") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.refid=data.toInt();
					}
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::FrameRef) {
				PropertyFrameRef *pp=static_cast<PropertyFrameRef*>(p);
				if (rPropertyInfo->getPropertySubName()=="ResourceFile") {
					pp->value.resourcefile=data.toStdString();
					rv=true;
				} else if (rPropertyInfo->getPropertySubName()=="TextureId") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.textureid=data.toInt();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Frame") {
					pp->value.frame=data.toStdString();
					rv=true;
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::RectInt) {
				PropertyRectInt *pp=static_cast<PropertyRectInt*>(p);
				if (rPropertyInfo->getPropertySubName()=="X") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.x=data.toInt();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Y") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.y=data.toInt();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Width") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.width=data.toInt();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Height") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.height=data.toInt();
					}
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::RectFloat) {
				PropertyRectFloat *pp=static_cast<PropertyRectFloat*>(p);
				if (rPropertyInfo->getPropertySubName()=="X") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.x=data.toFloat();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Y") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.y=data.toFloat();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Width") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.width=data.toFloat();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Height") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.height=data.toFloat();
					}
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::PointInt) {
				PropertyPointInt *pp=static_cast<PropertyPointInt*>(p);
				if (rPropertyInfo->getPropertySubName()=="X") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.x=data.toInt();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Y") {
					rv=validatePropertyInt(data);
					if (rv) {
						pp->value.y=data.toInt();
					}
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::PointFloat) {
				PropertyPointFloat *pp=static_cast<PropertyPointFloat*>(p);
				if (rPropertyInfo->getPropertySubName()=="X") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.x=data.toFloat();
					}
				} else if (rPropertyInfo->getPropertySubName()=="Y") {
					rv=validatePropertyFloat(data);
					if (rv) {
						pp->value.y=data.toFloat();
					}
				}
				if (rv) {
					QString rPropertyString=propertyToString(p);
					rPropertyInfo->getTreeWidgetItem()->setText(1,rPropertyString);
				}
			} else if (rPropertyType==PropertyType::List) {
				//PropertyList *pp=static_cast<PropertyList*>(p);
				rv=true;
			}
			GuiContext::getInstance().currentPropertyValueChanged(rPropertyInfo->getNode());
		}
	}
	if (rPropertyInfo && mNode && mNode->getNodeType()==NodeType::Layer || mNode->getNodeType()==NodeType::Sprite) {
		Node2d *rNode2d=static_cast<Node2d*>(mNode);
		rNode2d->synchronizeProperties();
	}
	return rv;
}

QString PropertyTreeDock::propertyToString(PropertyBase* p) {
	QString rv="";
	if (p) {
		PropertyType rPropertyType=p->getPropertyType();
		if (rPropertyType==PropertyType::Bool) {
			PropertyBool *pp=static_cast<PropertyBool*>(p);
			rv=pp->value==true?"true":"false";
		} else if (rPropertyType==PropertyType::Int) {
			PropertyInt *pp=static_cast<PropertyInt*>(p);
			rv=QString::fromStdString(std::to_string(pp->value));
		} else if (rPropertyType==PropertyType::String) {
			PropertyString *pp=static_cast<PropertyString*>(p);
			rv=QString::fromStdString(pp->value);
		} else if (rPropertyType==PropertyType::Float) {
			PropertyFloat *pp=static_cast<PropertyFloat*>(p);
			rv=QString::fromStdString(std::to_string(pp->value));
		} else if (rPropertyType==PropertyType::Ref) {
			PropertyRef *pp=static_cast<PropertyRef*>(p);
			rv=QString::fromStdString(
						   "File="+pp->value.reffile+
						   " | Id="+std::to_string(pp->value.refid));
		} else if (rPropertyType==PropertyType::FrameRef) {
			PropertyFrameRef *pp=static_cast<PropertyFrameRef*>(p);
			rv=QString::fromStdString(
						   "ResourceFile="+pp->value.resourcefile+
							" | TextureId="+std::to_string(pp->value.textureid) +
							" | Frame="+pp->value.frame
						);
		} else if (rPropertyType==PropertyType::RectInt) {
			PropertyRectInt *pp=static_cast<PropertyRectInt*>(p);
			rv=QString::fromStdString(
						   "X="+std::to_string(pp->value.x)+
						   " | Y="+std::to_string(pp->value.y)+
						   " | W="+std::to_string(pp->value.width)+
						   " | H="+std::to_string(pp->value.height));
		} else if (rPropertyType==PropertyType::RectFloat) {
			PropertyRectFloat *pp=static_cast<PropertyRectFloat*>(p);
			rv=QString::fromStdString(
						   "X="+std::to_string(pp->value.x)+
						   " | Y="+std::to_string(pp->value.y)+
						   " | W="+std::to_string(pp->value.width)+
						   " | H="+std::to_string(pp->value.height));
		} else if (rPropertyType==PropertyType::PointInt) {
			PropertyPointInt *pp=static_cast<PropertyPointInt*>(p);
			rv=QString::fromStdString(
						   "X="+std::to_string(pp->value.x)+
						   " | Y="+std::to_string(pp->value.y));
		} else if (rPropertyType==PropertyType::PointFloat) {
			PropertyPointFloat *pp=static_cast<PropertyPointFloat*>(p);
			rv=QString::fromStdString(
						   "X="+std::to_string(pp->value.x)+
						   " | Y="+std::to_string(pp->value.y));
		} else if (rPropertyType==PropertyType::List) {
			//PropertyList *pp=static_cast<PropertyList*>(p);
		}
	}
	return rv;
}

void PropertyTreeDock::addSubProperty(Node* rNode, PropertyBase* rPropertyBase, const string& propertyName, QTreeWidgetItem* rTreeWidgetItem, string rValue, const string &rPropertySubName) {
	QTreeWidgetItem* rTreeWidgetSubItem=nullptr;
	rTreeWidgetSubItem=new QTreeWidgetItem(rTreeWidgetItem);
	PropertyInfo rPropertyInfo(rNode, rPropertyBase, propertyName, rPropertySubName, rTreeWidgetItem, rTreeWidgetSubItem);
	setPropertyInfoToTreeItem(rTreeWidgetSubItem, rPropertyInfo);
	rTreeWidgetSubItem->setText(0,QString::fromStdString(rPropertySubName));
	rTreeWidgetSubItem->setText(1,QString::fromStdString(rValue));
	rTreeWidgetSubItem->setFlags(rTreeWidgetSubItem->flags()|Qt::ItemIsEditable);
	rTreeWidgetItem->addChild(rTreeWidgetSubItem);
	rTreeWidgetItem->setExpanded(true);
	//r->setExpanded(true);
}

void PropertyTreeDock::setPropertiesForNode(Node* rNode) {
	clear();
	mNode=rNode;
	if (rNode) {
		unsigned long count=rNode->getPropertyCount();
		for (unsigned long i=0;i<count;i++) {
			PropertyBase* p=rNode->getPropertyFromIndex(i);
			if (p) {
				QTreeWidgetItem* r=nullptr;
				PropertyInfo rPropertyInfo(rNode, p, p->getPropertyName(), "", r, nullptr);
				r=new QTreeWidgetItem(ui->propertyTree);
				setPropertyInfoToTreeItem(r, rPropertyInfo);
				PropertyType rPropertyType=p->getPropertyType();
				if (rPropertyType==PropertyType::Bool) {
					//PropertyBool *pp=static_cast<PropertyBool*>(p);
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::Int) {
					//PropertyInt *pp=static_cast<PropertyInt*>(p);
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::String) {
					//PropertyString *pp=static_cast<PropertyString*>(p);
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::Float) {
					//PropertyFloat *pp=static_cast<PropertyFloat*>(p);
					r->setFlags(r->flags()|Qt::ItemIsEditable);
				} else if (rPropertyType==PropertyType::Ref) {
					PropertyRef *pp=static_cast<PropertyRef*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, pp->value.reffile, "File");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.refid), "Id");
				} else if (rPropertyType==PropertyType::FrameRef) {
					PropertyFrameRef *pp=static_cast<PropertyFrameRef*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, pp->value.resourcefile, "ResourceFile");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.textureid), "TextureId");
					addSubProperty(rNode, p, pp->getPropertyName(), r, pp->value.frame, "Frame");
				} else if (rPropertyType==PropertyType::RectInt) {
					PropertyRectInt *pp=static_cast<PropertyRectInt*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.x), "X");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.y), "Y");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.width), "Width");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.height), "Height");
				} else if (rPropertyType==PropertyType::RectFloat) {
					PropertyRectFloat *pp=static_cast<PropertyRectFloat*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.x), "X");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.y), "Y");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.width), "Width");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.height), "Height");
				} else if (rPropertyType==PropertyType::PointInt) {
					PropertyPointInt *pp=static_cast<PropertyPointInt*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.x), "X");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.y), "Y");
				} else if (rPropertyType==PropertyType::PointFloat) {
					PropertyPointFloat *pp=static_cast<PropertyPointFloat*>(p);
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.x), "X");
					addSubProperty(rNode, p, pp->getPropertyName(), r, std::to_string(pp->value.y), "Y");
				} else if (rPropertyType==PropertyType::List) {
					//PropertyList *pp=static_cast<PropertyList*>(p);
				}
				r->setText(0,QString::fromStdString(p->getPropertyName()));
				QString rPropertyString=propertyToString(p);
				r->setText(1,rPropertyString);

				ui->propertyTree->addTopLevelItem(r);
			}
		}
	}
}

void PropertyTreeDock::on_propertyTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	ignoreparam(item);
	ignoreparam(column);
	/*if (column==1) {
		ui->propertyTree->setItemDelegateForColumn(column, mPropertyNumberDelegate);
		ui->propertyTree->editItem(item, column);
	}*/
}

void PropertyTreeDock::on_propertyTree_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column==1) {
		ui->propertyTree->setItemDelegateForColumn(column, mPropertyNumberDelegate);
		ui->propertyTree->editItem(item, column);
	}
}
