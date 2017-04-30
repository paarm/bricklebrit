#pragma once

#include "propertybase.h"
#include <vector>

typedef vector<PropertyBase*> ListType;

class PropertyList: public PropertyBaseValue<ListType> {
public:
	PropertyList();
	~PropertyList ();
	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
	unsigned long entryCount();
	void addEntry(PropertyBase* rEntry);
	PropertyBase* getEntryFromIndex(unsigned long rIndex);
	void removeEntryFromIndex(unsigned long rIndex);
};


