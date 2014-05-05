#ifndef _INDEX_H_
#define _INDEX_H_

#include "GeoUtility.h"

class Index{
private:
public:
	Index(){}
	virtual bool insert(const Point* point) = 0;
	virtual bool hasPointInTri(Triangle* triangle) = 0;
	virtual void pointInTri(Triangle* triangle, vector<Point*>* v) = 0;
	virtual bool remove(const Point* point) = 0;
};

#endif