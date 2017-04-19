#pragma once
#include <string>
#include "../../jsonparser/jsonparser.h"

using namespace std;

enum class PropertyType {
	Integer,
	String,
	Float,
	RectInt,
	RectFloat,
	PointInt,
	PointFloat
};

PropertyType getPropertyTypeFromName(const string &rType);
const string getPropertyStringFromPropertyType(const PropertyType& rPropertyType);

struct PropertyBase {
	PropertyType	propertyType;
	string			name;
	string getSerialStart(string rTypeString) {
		return string("")+"{ \"name\": \""+name+"\", \"type\": \""+rTypeString+"\", ";
	}
	string getSerialEnd() {
		return string("}");
	}
	virtual string serialize()=0;
	virtual void deserialize(JSONValue *rPropertyValue)=0;
	virtual ~PropertyBase() {}
};

template <typename T>
struct PropertyBaseValue : PropertyBase {
	T value;
};
