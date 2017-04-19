#pragma once
#include "propertybase.h"

struct PropertyFloat : PropertyBaseValue<float> {
	virtual string serialize() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": "+to_string(value)
				+getSerialEnd();
	}
	virtual void deserialize(JSONValue *rPropertyValueParent) override {
		float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
		value=rValue;
	}

};
