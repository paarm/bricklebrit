#include "property_list.h"


PropertyList::PropertyList() : PropertyBaseValue(PropertyType::List) {
}

PropertyList::~PropertyList () {
	value.push_back(nullptr);
	for(PropertyBase* rEntry : value) {
		delete rEntry;
	}
}

string PropertyList::serializeValue() {
	string rv=getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
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

void PropertyList::deserializeValue(JSONValue *rPropertyValueParent) {
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

unsigned long PropertyList::entryCount() {
	return value.size();
}
void PropertyList::addEntry(PropertyBase* rEntry) {
	value.push_back(rEntry);
}
PropertyBase* PropertyList::getEntryFromIndex(unsigned long rIndex) {
	if (value.size()>rIndex) {
		return value[rIndex];
	}
	return nullptr;
}
void PropertyList::removeEntryFromIndex(unsigned long rIndex) {
	if (value.size()>rIndex) {
		delete value[rIndex];
		value.erase(value.begin()+static_cast<long>(rIndex));
	}
}
