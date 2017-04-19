#pragma once
#include "propertybase.h"

template <typename T>
struct Rect {
	T x;
	T y;
	T width;
	T height;
	Rect() {}
	Rect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}

	string getSerialString() {
		return string()+"\"value\": {"+
							+"\"x\": "+to_string(x)
							+",\"y\": "+to_string(y)
							+",\"width\": "+to_string(width)
							+",\"height\": "+to_string(height)
						+"}";
	}
};


typedef Rect<int> RectInt;

struct PropertyRectInt : PropertyBaseValue<RectInt> {
	virtual string serialize() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+value.getSerialString()
				+getSerialEnd();
	}
	virtual void deserialize(JSONValue *rPropertyValueParent) override {
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
};

typedef Rect<float> RectFloat;

struct PropertyRectFloat : PropertyBaseValue<RectFloat> {
	virtual string serialize() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+value.getSerialString()
				+getSerialEnd();
	}
	virtual void deserialize(JSONValue *rPropertyValueParent) override {
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

};
