#pragma once
#include "propertybase.h"

struct PropertyInt : PropertyBaseValue<int> {
public:
	PropertyInt();
	PropertyInt(const int &v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
