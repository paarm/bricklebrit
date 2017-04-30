#include "property_rect.h"

PropertyRectInt::PropertyRectInt() : PropertyBaseValue(PropertyType::RectInt) {
}

PropertyRectInt::PropertyRectInt(int rX, int rY, int rWidth, int rHeight) : PropertyRectInt() {
	value.x=rX;
	value.y=rY;
	value.width=rWidth;
	value.height=rHeight;
}

PropertyRectInt::PropertyRectInt(const RectInt &v) : PropertyRectInt() {
	value=v;
}

string PropertyRectInt::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyRectInt::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		float rX=JsonParserBase::extractNumber(values, L"x");
		value.x=static_cast<int>(rX);
		float rY=JsonParserBase::extractNumber(values, L"y");
		value.y=static_cast<int>(rY);
		float rWidth=JsonParserBase::extractNumber(values, L"width");
		value.width=static_cast<int>(rWidth);
		float rHeight=JsonParserBase::extractNumber(values, L"height");
		value.height=static_cast<int>(rHeight);
	}
}



PropertyRectFloat::PropertyRectFloat() : PropertyBaseValue(PropertyType::RectFloat) {
}

PropertyRectFloat::PropertyRectFloat(float rX, float rY, float rWidth, float rHeight) : PropertyRectFloat() {
	value.x=rX;
	value.y=rY;
	value.width=rWidth;
	value.height=rHeight;
}

PropertyRectFloat::PropertyRectFloat(const RectFloat &v) : PropertyRectFloat() {
	value=v;
}

string PropertyRectFloat::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyRectFloat::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		float rX=JsonParserBase::extractNumber(values, L"x");
		value.x=rX;
		float rY=JsonParserBase::extractNumber(values, L"y");
		value.y=rY;
		float rWidth=JsonParserBase::extractNumber(values, L"width");
		value.width=rWidth;
		float rHeight=JsonParserBase::extractNumber(values, L"height");
		value.height=rHeight;
	}
}
