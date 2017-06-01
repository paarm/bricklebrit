#include "property_ref.h"

PropertyRef::PropertyRef() : PropertyBaseValue(PropertyType::Ref) {
}

PropertyRef::PropertyRef(string rRefFile, int rRefId) : PropertyRef() {
	value.reffile=rRefFile;
	value.refid=rRefId;
}

PropertyRef::PropertyRef(const Ref &v) : PropertyRef() {
	value=v;
}

string PropertyRef::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyRef::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		string rRefFile=JsonParserBase::extractString(values, L"reffile");
		value.reffile=rRefFile;
		float rRefId=JsonParserBase::extractNumber(values, L"refid");
		value.refid=static_cast<int>(rRefId);
	}
}
