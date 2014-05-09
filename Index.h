#ifndef _INDEX_H_
#define _INDEX_H_

#include "GeoUtility.h"

class Index{
private:
public:
	Index(){}
	virtual const Point* insert(Point* point) = 0;
	virtual bool hasPointInTri(const Triangle* triangle) = 0;
	virtual void pointInTri(const Triangle* triangle, vector<Point*>* v) = 0;
	virtual bool remove(const Point* point) = 0;
};

#endif