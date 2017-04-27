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
	PropertyPointInt() {
		value.x=0;
		value.y=0;
		propertyType=PropertyType::PointInt;
	}
	PropertyPointInt(int rX, int rY) {
		value.x=rX;
		value.y=rY;
		propertyType=PropertyType::PointInt;
	}

	PropertyPointInt(const PointInt& v) {
		value=v;
		propertyType=PropertyType::PointInt;
	}
	virtual string serializeValue() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+value.getSerialString();
	}
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
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
	PropertyPointFloat() {
		value.x=0.0;
		value.y=0.0;
		propertyType=PropertyType::PointFloat;
	}
	PropertyPointFloat(float rX, float rY) {
		value.x=rX;
		value.y=rY;
		propertyType=PropertyType::PointFloat;
	}
	PropertyPointFloat(const PointFloat& v) {
		value=v;
		propertyType=PropertyType::PointFloat;
	}
	virtual string serializeValue() override {
		return getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+value.getSerialString();
	}
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
		JSONValue *values=rPropertyValueParent->Child(L"value");
		if (values && values->IsObject()) {
			float rX=JsonParserBase::extractNumber(values, L"x");
			value.x=rX;
			float rY=JsonParserBase::extractNumber(values, L"y");
			value.y=rY;
		}
	}
};
