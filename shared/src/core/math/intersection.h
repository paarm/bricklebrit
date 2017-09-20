#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "../tree/property/property_point.h"

class Intersection
{
public:
	static bool isPolygonsIntersecting(vector<PointFloat> &a, vector<PointFloat> &b);
};

#endif // INTERSECTION_H
