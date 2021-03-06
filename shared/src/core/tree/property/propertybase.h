#pragma once
#include <string>
#include "../../jsonparser/jsonparser.h"

using namespace std;

enum class PropertyType {
	Int,
	Bool,
	String,
	Float,
	Ref,
	FrameRef,
	RectInt,
	RectFloat,
	PointInt,
	PointFloat,
	List
};

class PropertyBase {
private:
	PropertyType	mPropertyType;
	string			mPropertyName;
public:
	PropertyBase(PropertyType rPropertyType);
	void setPropertyName(const string& rPropertyName);
	string getPropertyName();
	PropertyType getPropertyType();
	string getSerialStart(string rTypeString);
	virtual string serializeValue()=0;
	virtual void deserializeValue(JSONValue *rPropertyValue)=0;
	virtual ~PropertyBase();
};

template <typename T>
class PropertyBaseValue : public PropertyBase {
public:
	using PropertyBase::PropertyBase;
	T value;
};

PropertyType getPropertyTypeFromName(const string &rType);
const string getPropertyStringFromPropertyType(const PropertyType& rPropertyType);
PropertyBase* getPropertyInstanceByType(PropertyType rPropertyType);


#define PROPERTY_STRING_GETSET(fieldName) \
void set##fieldName(const string& r##fieldName) { \
	setPropertyString (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyString (#fieldName); \
} \
const string& get##fieldName() { \
	return getPropertyString (#fieldName)->value; \
}

#define PROPERTY_FLOAT_GETSET(fieldName) \
void set##fieldName(const float& r##fieldName) { \
	setPropertyFloat (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyFloat (#fieldName); \
} \
float& get##fieldName() { \
	return getPropertyFloat (#fieldName)->value; \
}

#define PROPERTY_BOOL_GETSET(fieldName) \
void set##fieldName(const bool& r##fieldName) { \
	setPropertyBool (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyBool (#fieldName); \
} \
bool& get##fieldName() { \
	return getPropertyBool (#fieldName)->value; \
}

#define PROPERTY_INT_GETSET(fieldName) \
void set##fieldName(const int& r##fieldName) { \
	setPropertyInt (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyInt (#fieldName); \
} \
int& get##fieldName() { \
	return getPropertyInt (#fieldName)->value; \
}

#define PROPERTY_REF_GETSET(fieldName) \
void set##fieldName(const Ref& r##fieldName) { \
	setPropertyRef (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyRef (#fieldName); \
} \
Ref& get##fieldName() { \
	return getPropertyRef (#fieldName)->value; \
}

#define PROPERTY_FRAMEREF_GETSET(fieldName) \
void set##fieldName(const FrameRef& r##fieldName) { \
	setPropertyFrameRef (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyFrameRef (#fieldName); \
} \
FrameRef& get##fieldName() { \
	return getPropertyFrameRef (#fieldName)->value; \
}

#define PROPERTY_RECTINT_GETSET(fieldName) \
void set##fieldName(const RectInt& r##fieldName) { \
	setPropertyRectInt (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyRectInt (#fieldName); \
} \
RectInt& get##fieldName() { \
	return getPropertyRectInt (#fieldName)->value; \
}

#define PROPERTY_RECTFLOAT_GETSET(fieldName) \
void set##fieldName(const RectFloat& r##fieldName) { \
	setPropertyRectFloat (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyRectFloat (#fieldName); \
} \
RectFloat& get##fieldName() { \
	return getPropertyRectFloat (#fieldName)->value; \
}

#define PROPERTY_POINTINT_GETSET(fieldName) \
void set##fieldName(const PointInt& r##fieldName) { \
	setPropertyPointInt (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyPointInt (#fieldName); \
} \
PointInt& get##fieldName() { \
	return getPropertyPointInt (#fieldName)->value; \
}

#define PROPERTY_POINTFLOAT_GETSET(fieldName) \
void set##fieldName(const PointFloat& r##fieldName) { \
	setPropertyPointFloat (#fieldName, r##fieldName); \
} \
void set##fieldName() { \
	setPropertyPointFloat (#fieldName); \
} \
const PointFloat& get##fieldName() { \
	return getPropertyPointFloat (#fieldName)->value; \
}

#define PROPERTY_LIST_GETSET(fieldName) \
void set##fieldName() { \
	setPropertyList (#fieldName); \
} \
const ListType& get##fieldName() { \
	return getPropertyList (#fieldName)->value; \
} \
PropertyList* getPropertyList##fieldName() { \
	return getPropertyList (#fieldName); \
}


