#pragma once
#include "propertybase.h"

template <typename T>
struct Point {
	T x=static_cast<T>(0);
	T y=static_cast<T>(0);
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
typedef Point<float> PointFloat;

class PropertyPointInt : public PropertyBaseValue<PointInt> {
public:
	PropertyPointInt();
	PropertyPointInt(int rX, int rY);
	PropertyPointInt(const PointInt& v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};


class PropertyPointFloat : public PropertyBaseValue<PointFloat> {
public:
	PropertyPointFloat();
	PropertyPointFloat(float rX, float rY);
	PropertyPointFloat(const PointFloat& v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
