#pragma once

#include "propertybase.h"
#include <vector>

typedef vector<PropertyBase*> ListType;

struct PropertyList: PropertyBaseValue<ListType> {
	PropertyList() {
		propertyType=PropertyType::List;
	}

	~PropertyList () {
		value.push_back(nullptr);
		for(PropertyBase* rEntry : value) {
			delete rEntry;
		}
	}

	virtual string serializeValue() override {
		string rv=getSerialStart(getPropertyStringFromPropertyType(propertyType))
				+"\"value\": [";
		bool first=true;
		for(PropertyBase* rEntry : value) {
			if (!first) {
				rv+=",";
			}
			rv+="{ "+rEntry->serializeValue()+" }";
			first=false;
		}
		rv+="]";
		return rv;
	}

	virtual void deserializeValue(JSONValue *rPropertyValueParent) override {
		// values is a array, every entry is a property_xx
		JSONValue *values=rPropertyValueParent->Child(L"value");
		if (values && values->IsArray()) {
			const JSONArray rJSONArray=values->AsArray();
			for (auto *rJSONValue : rJSONArray) {
				string rType=JsonParserBase::extractString(rJSONValue, L"type");
				if (rType.empty()) {
					cout << "Properties.list.type not found or empty\n";
					continue;
				}
				PropertyType rPropertyType=getPropertyTypeFromName(rType);
				PropertyBase* p=nullptr;
				p=getPropertyInstanceByType(rPropertyType);
				if (p) {
					p->deserializeValue(rJSONValue);
					addEntry(p);
				}
			}
		}
	}

	unsigned long entryCount() {
		return value.size();
	}
	void addEntry(PropertyBase* rEntry) {
		value.push_back(rEntry);
	}
	PropertyBase* getEntryFromIndex(unsigned long rIndex) {
		if (value.size()>rIndex) {
			return value[rIndex];
		}
		return nullptr;
	}
	void removeEntryFromIndex(unsigned long rIndex) {
		if (value.size()>rIndex) {
			delete value[rIndex];
			value.erase(value.begin()+static_cast<long>(rIndex));
		}
	}
};


