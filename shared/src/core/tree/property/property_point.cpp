#include "property_point.h"

PropertyPointInt::PropertyPointInt() : PropertyBaseValue(PropertyType::PointInt) {
}

PropertyPointInt::PropertyPointInt(int rX, int rY) : PropertyPointInt() {
	value.x=rX;
	value.y=rY;
}

PropertyPointInt::PropertyPointInt(const PointInt& v) : PropertyPointInt() {
	value=v;
}

string PropertyPointInt::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyPointInt::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		float rX=JsonParserBase::extractNumber(values, L"x");
		value.x=static_cast<int>(rX);
		float rY=JsonParserBase::extractNumber(values, L"y");
		value.y=static_cast<int>(rY);
	}
}



PropertyPointFloat::PropertyPointFloat() : PropertyBaseValue (PropertyType::PointFloat) {
}

PropertyPointFloat::PropertyPointFloat(float rX, float rY) : PropertyPointFloat() {
	value.x=rX;
	value.y=rY;
}

PropertyPointFloat::PropertyPointFloat(const PointFloat& v) :  PropertyPointFloat() {
	value=v;
}

string PropertyPointFloat::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyPointFloat::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		float rX=JsonParserBase::extractNumber(values, L"x");
		value.x=rX;
		float rY=JsonParserBase::extractNumber(values, L"y");
		value.y=rY;
	}
}
