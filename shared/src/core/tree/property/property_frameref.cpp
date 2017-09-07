#include "property_frameref.h"

PropertyFrameRef::PropertyFrameRef() : PropertyBaseValue(PropertyType::FrameRef) {
}

PropertyFrameRef::PropertyFrameRef(string rResourceFile, int rTextureId, string rFrame) : PropertyFrameRef() {
	value.resourcefile=rResourceFile;
	value.textureid=rTextureId;
	value.frame=rFrame;
}

PropertyFrameRef::PropertyFrameRef(const FrameRef &v) : PropertyFrameRef() {
	value=v;
}

string PropertyFrameRef::serializeValue() {
	return getSerialStart(getPropertyStringFromPropertyType(getPropertyType()))
			+value.getSerialString();
}

void PropertyFrameRef::deserializeValue(JSONValue *rPropertyValueParent) {
	JSONValue *values=rPropertyValueParent->Child(L"value");
	if (values && values->IsObject()) {
		string rResourceFile=JsonParserBase::extractString(values, L"resourcefile");
		value.resourcefile=rResourceFile;
		float rTextureId=JsonParserBase::extractNumber(values, L"textureid");
		value.textureid=static_cast<int>(rTextureId);
		string rFrame=JsonParserBase::extractString(values, L"frame");
		value.frame=rFrame;
	}
}
