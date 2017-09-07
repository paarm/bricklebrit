#pragma once
#include "propertybase.h"


struct FrameRef {
	string	resourcefile;
	int		textureid=0;
	string	frame;

	FrameRef () {
	}

	FrameRef(string rResouceFile, int rTextureId, string rFrame) :
		resourcefile(rResouceFile),
		textureid(rTextureId),
		frame(rFrame) {}

	string getSerialString() {
		return string()+"\"value\": {"+
							+"\"resourcefile\": \""+resourcefile+"\""
							+",\"textureid\": "+to_string(textureid)
							+",\"frame\": \""+frame+"\""
						+"}";
	}
};


class PropertyFrameRef : public PropertyBaseValue<FrameRef> {
public:
	PropertyFrameRef();
	PropertyFrameRef(string rResourceFile, int rTextureId, string rFrame);
	PropertyFrameRef(const FrameRef &v);

	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
