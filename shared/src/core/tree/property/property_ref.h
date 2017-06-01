#pragma once
#include "propertybase.h"


struct Ref {
	string	reffile;
	int		refid=0;

	Ref () {
	}

	Ref(string rRefFile, int rRefId) : reffile(rRefFile), refid(rRefId) {}

	string getSerialString() {
		return string()+"\"value\": {"+
							+"\"reffile\": \""+reffile+"\""
							+",\"refid\": "+to_string(refid)
						+"}";
	}
};


class PropertyRef : public PropertyBaseValue<Ref> {
public:
	PropertyRef();
	PropertyRef(string rRefFile, int rRefId);
	PropertyRef(const Ref &v);

	virtual string serializeValue() override;
	virtual void deserializeValue(JSONValue *rPropertyValueParent) override;
};
