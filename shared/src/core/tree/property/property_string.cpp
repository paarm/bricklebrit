#include "property_string.h"

PropertyString::PropertyString() : PropertyBaseValue(PropertyType::String) {
	value="";
}

PropertyString::PropertyString(const string &v) : PropertyString() {
	value=v;
}

string PropertyString::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))+"\"value\": \""+value+"\"";
}
void PropertyString::deserializeValue(JSONValue *rPropertyValueParent) {
	string rValue=JsonParserBase::extractString(rPropertyValueParent, L"value");
	value=rValue;
}
