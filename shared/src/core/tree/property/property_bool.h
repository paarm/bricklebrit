#pragma once
#include "propertybase.h"

class PropertyBool : public PropertyBaseValue<bool> {
public:
	PropertyBool();
	PropertyBool(const bool &v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};

