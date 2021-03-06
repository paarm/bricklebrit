#pragma once
#include "propertybase.h"

class PropertyFloat : public PropertyBaseValue<float> {
public:
	PropertyFloat();
	PropertyFloat(const float &v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
