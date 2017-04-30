#pragma once
#include "propertybase.h"

template <typename T>
struct Rect {
	T x=static_cast<T>(0);
	T y=static_cast<T>(0);
	T width=static_cast<T>(0);
	T height=static_cast<T>(0);

	Rect () {
	}

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
typedef Rect<float> RectFloat;

class PropertyRectInt : public PropertyBaseValue<RectInt> {
public:
	PropertyRectInt();
	PropertyRectInt(int rX, int rY, int rWidth, int rHeight);
	PropertyRectInt(const RectInt &v);

	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};


class PropertyRectFloat : public PropertyBaseValue<RectFloat> {
public:
	PropertyRectFloat();
	PropertyRectFloat(float rX, float rY, float rWidth, float rHeight);
	PropertyRectFloat(const RectFloat &v);
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
