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
	PointFloat,
	List
};

PropertyType getPropertyTypeFromName(const string &rType);
const string getPropertyStringFromPropertyType(const PropertyType& rPropertyType);

struct PropertyBase {
	PropertyType	propertyType;
	string getSerialStart(string rTypeString) {
		return string("")+"\"type\": \""+rTypeString+"\", ";
	}
	virtual string serializeValue()=0;
	virtual void deserializeValue(JSONValue *rPropertyValue)=0;
	virtual ~PropertyBase() {}
};

template <typename T>
struct PropertyBaseValue : PropertyBase {
	T value;
};

PropertyBase* getPropertyInstanceByType(PropertyType rPropertyType);



