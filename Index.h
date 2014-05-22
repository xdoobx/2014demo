#ifndef _INDEX_H_
#define _INDEX_H_

#include "GeoUtility.h"

class Index{
private:
public:
	Rect range;
	int size;

	Point* point;
	Index* subTree[4];

	Index(){}
	Index(LineSet* map){};
	Index(LineSet* map, PointSet* points){}

	virtual inline const Point* insert(Point* point) = 0;
	virtual bool hasPointInTri(const Triangle* triangle) = 0;
	virtual void pointInTri(const Triangle* triangle, vector<Point*>* v) = 0;
	virtual bool remove(const Point* point) = 0;
};

#endif