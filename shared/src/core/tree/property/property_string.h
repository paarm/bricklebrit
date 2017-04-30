#pragma once
#include "propertybase.h"

class PropertyString : public PropertyBaseValue<string> {
public:
	PropertyString();
	PropertyString(const string &v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
