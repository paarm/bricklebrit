#include "property_bool.h"

PropertyBool::PropertyBool() : PropertyBaseValue(PropertyType::Bool) {
	value=false;
}

PropertyBool::PropertyBool(const bool &v) : PropertyBool() {
	value=v;
}
string PropertyBool::serializeValue() {

	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+"\"value\": "+(value?"true":"false");//to_string(value);
}
void PropertyBool::deserializeValue(JSONValue *rPropertyValueParent) {
	bool rValue=JsonParserBase::extractBool(rPropertyValueParent, L"value");
	value=rValue;
}
