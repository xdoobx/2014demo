#ifndef _INDEX_H_
#define _INDEX_H_

#include "GeoUtility.h"

class Index{
private:
public:
	Rect range; //range of current branch node
	int size; // number of points under the node

	Point* point; //if it is a leaf node, it should record a point, otherwise null
	Index* subTree[4]; //any index should guarantee they have 4 subtree under root node
	Index** sub; //for any other child classes don't want exactly 4 subtrees.

	Index(){}

	virtual inline const Point* insert(Point* point) = 0;
	//whether there is any point in a given triangle
	virtual bool hasPointInTri(const Triangle* triangle) = 0;
	//find all points in a given triangle
	virtual void pointInTri(const Triangle* triangle, vector<Point*>* v) = 0;
	virtual bool remove(const Point* point) = 0;
};

#endif