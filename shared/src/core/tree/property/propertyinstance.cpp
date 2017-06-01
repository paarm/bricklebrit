#include "property_int.h"
#include "property_bool.h"
#include "property_float.h"
#include "property_string.h"
#include "property_point.h"
#include "property_rect.h"
#include "property_list.h"
#include "property_ref.h"

PropertyBase* getPropertyInstanceByType(PropertyType rPropertyType) {
	PropertyBase* p=nullptr;
	if (rPropertyType==PropertyType::Bool) {
		p=new PropertyBool();
	} else if (rPropertyType==PropertyType::Float) {
		p=new PropertyFloat();
	} else if (rPropertyType==PropertyType::Int) {
		p=new PropertyInt(0);
	} else if (rPropertyType==PropertyType::Ref) {
		p=new PropertyRef();
	} else if (rPropertyType==PropertyType::String) {
		p=new PropertyString();
	} else if (rPropertyType==PropertyType::PointInt) {
		p=new PropertyPointInt();
	} else if (rPropertyType==PropertyType::PointFloat) {
		p=new PropertyPointFloat();
	} else if (rPropertyType==PropertyType::RectInt) {
		p=new PropertyRectInt();
	} else if (rPropertyType==PropertyType::RectFloat) {
		p=new PropertyRectFloat();
	} else if (rPropertyType==PropertyType::List) {
		p=new PropertyList();
	}
	return p;
}
