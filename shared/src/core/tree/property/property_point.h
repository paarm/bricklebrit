#pragma once
#include "propertybase.h"

template <typename T>
struct Point {
	T x;
	T y;
	Point() {}
	Point(T x, T y) : x(x), y(y) {}

	string getSerialString() {
		return string()+
				"\"value\": {"+
				+"\"x\": "+to_string(x)
				+",\"y\": "+to_string(y)
				+"}";
	}
};

typedef Point<int> PointInt;

struct PropertyPointInt : PropertyBaseValue<PointInt> {
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
		}
	}
};

typedef Point<float> PointFloat;

struct PropertyPointFloat : PropertyBaseValue<PointFloat> {
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
		}
	}
};
