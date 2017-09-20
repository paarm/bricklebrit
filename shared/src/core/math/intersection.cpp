#include "intersection.h"

bool Intersection::isPolygonsIntersecting(vector<PointFloat> &a, vector<PointFloat> &b) {
	for (int x=0; x<2; x++) {
		vector<PointFloat>&polygon = (x==0) ? a : b;
		for (unsigned int i1=0; i1<polygon.size(); i1++) {
			int   i2 = (i1 + 1) % polygon.size();
			PointFloat &p1 = polygon.at(i1);
			PointFloat &p2 = polygon.at(i2);
			PointFloat normal = PointFloat(p2.y - p1.y, p1.x - p2.x);

			bool haveMinA=false;
			bool haveMaxA=false;
			float minA=0.0f;
			float maxA=0.0f;

			for (PointFloat &p : a) {
				float projected = normal.x * p.x + normal.y * p.y;
				if (!haveMinA || projected < minA) {
					haveMinA=true;
					minA = projected;
				}
				if (!haveMaxA || projected > maxA) {
					haveMaxA=true;
					maxA = projected;
				}
			}

			bool haveMinB=false;
			bool haveMaxB=false;
			float minB=0.0f;
			float maxB=0.0f;

			for (PointFloat &p : b) {
				float projected = normal.x * p.x + normal.y * p.y;
				if (!haveMinB || projected < minB) {
					haveMinB=true;
					minB = projected;
				}
				if (!haveMaxB || projected > maxB) {
					haveMaxB=true;
					maxB = projected;
				}
			}

			if (maxA < minB || maxB < minA) {
				return false;
			}
		}
	}
	return true;
}
