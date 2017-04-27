#pragma once
#include "propertybase.h"

struct PropertyInt : PropertyBaseValue<int> {
	PropertyInt() {
		value=0;
		propertyType=PropertyType::PointInt;
	}

	PropertyInt(const int &v) {
		value=v;
		propertyType=PropertyType::PointInt;
	}
	virtual string serializeValue() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": "+to_string(value);
	}
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
		float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
		value=(int)rValue;
	}

};
