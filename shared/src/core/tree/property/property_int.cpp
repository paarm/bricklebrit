#include "property_int.h"

PropertyInt::PropertyInt() : PropertyBaseValue(PropertyType::Int) {
	value=0;
}

PropertyInt::PropertyInt(const int &v) : PropertyInt() {
	value=v;
}
string PropertyInt::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+"\"value\": "+to_string(value);
}
void PropertyInt::deserializeValue(JSONValue *rPropertyValueParent) {
	float rValue=JsonParserBase::extractNumber(rPropertyValueParent, L"value");
	value=static_cast<int>(rValue);
}
