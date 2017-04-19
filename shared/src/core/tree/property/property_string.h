#pragma once
#include "propertybase.h"

struct PropertyString : PropertyBaseValue<string> {
	virtual string serialize() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": \""+value+"\""
				+getSerialEnd();
	}
	virtual void deserialize(JSONValue *rPropertyValueParent) override {
		string rValue=JsonParserBase::extractString(rPropertyValueParent, L"value");
		value=rValue;
	}
};
