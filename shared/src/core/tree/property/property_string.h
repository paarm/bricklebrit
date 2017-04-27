#pragma once
#include "propertybase.h"

struct PropertyString : PropertyBaseValue<string> {
	PropertyString() {
		value="";
		propertyType=PropertyType::String;
	}

	PropertyString(const string &v) {
		value=v;
		propertyType=PropertyType::String;
	}

	virtual string serializeValue() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": \""+value+"\"";
	}
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
		string rValue=JsonParserBase::extractString(rPropertyValueParent, L"value");
		value=rValue;
	}
};
