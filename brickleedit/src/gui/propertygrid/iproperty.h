#pragma once
#include "propertyinfo.h"
#include <QObject>

class IProperty {
public:
	virtual bool onPropertyChange(PropertyInfo* rPropertyInfo, QString data)=0;
};
