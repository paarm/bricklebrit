#include "property_float.h"

PropertyFloat::PropertyFloat() : PropertyBaseValue(PropertyType::Float) {
	value=0.0;
}

PropertyFloat::PropertyFloat(const float &v) : PropertyFloat() {
	value=v;
}

string PropertyFloat::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+"\"value\": "+to_string(value);
}

void PropertyFloat::deserializeValue(JSONValue *rPropertyValueParent) {
	float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
	value=rValue;
}
