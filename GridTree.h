#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <thread>
#include <vector>
#include "GeoUtility.h"

class GridTree{
private:
public:
	Rect range; //range of current branch node
	int size; // number of points under the node
	int divideW;
	double gridW;
	int divideH;
	double gridH;

	vector<Point*>** grid;
	GridTree(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points

	inline const Point* insert(Point* point);
	virtual inline bool hasPointInTri(const Triangle* triangle);
	inline vector<Point*> pointsInTri(const Triangle* triangle);
};

#endif