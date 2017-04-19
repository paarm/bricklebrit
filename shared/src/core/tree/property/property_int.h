#pragma once
#include "propertybase.h"

struct PropertyInt : PropertyBaseValue<int> {
	virtual string serialize() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": "+to_string(value)
				+getSerialEnd();
	}
	virtual void deserialize(JSONValue *rPropertyValueParent) override {
		float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
		value=(int)rValue;
	}

};
