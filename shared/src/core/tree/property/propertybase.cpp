#include "propertybase.h"

PropertyType getPropertyTypeFromName(const string &rType) {
	if (rType=="Integer") {
		return PropertyType::Integer;
	} else if (rType=="String") {
		return PropertyType::String;
	} else if (rType=="Float") {
		return PropertyType::Float;
	} else if (rType=="RectInt") {
		return PropertyType::RectInt;
	} else if (rType=="RectFloat") {
		return PropertyType::RectFloat;
	} else if (rType=="PointInt") {
		return PropertyType::PointInt;
	} else if (rType=="PointFloat") {
		return PropertyType::PointFloat;
	} else if (rType=="List") {
		return PropertyType::List;
	}
	return PropertyType::String;
}

const string getPropertyStringFromPropertyType(const PropertyType& rPropertyType) {
	if (rPropertyType==PropertyType::Integer) {
		return "Integer";
	} else if (rPropertyType==PropertyType::String) {
		return "String";
	} else if (rPropertyType==PropertyType::Float) {
		return "Float";
	} else if (rPropertyType==PropertyType::RectInt) {
		return "RectInt";
	} else if (rPropertyType==PropertyType::RectFloat) {
		return "RectFloat";
	} else if (rPropertyType==PropertyType::PointInt) {
		return "PointInt";
	} else if (rPropertyType==PropertyType::PointFloat) {
		return "PointFloat";
	} else if (rPropertyType==PropertyType::List) {
		return "List";
	}
	return "String";
}

