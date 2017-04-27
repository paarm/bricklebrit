#pragma once
#include "propertybase.h"

struct PropertyFloat : PropertyBaseValue<float> {
	PropertyFloat() {
		value=0.0;
		propertyType=PropertyType::Float;
	}

	PropertyFloat(const float &v) {
		value=v;
		propertyType=PropertyType::Float;
	}

	virtual string serializeValue() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": "+to_string(value);
	}
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
		float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
		value=rValue;
	}

};
