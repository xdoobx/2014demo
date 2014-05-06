#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Index.h"

class QuadTree: private Index
{
private:
	bool subDivid();
	inline bool isInTri(Triangle* triangle, double x, double y);
	inline bool isCross(Point* p1, Point* p2,
		const double& p3x, const double& p3y, const double& p4x, const double& p4y);
	inline bool isIntersect(Triangle* triangle);
public:
	Rect range;
	int size;
	const Point* point;

	QuadTree* subTree[4];

	QuadTree(Rect rect);
	QuadTree(LineSet* map);
	QuadTree(LineSet* map, int mark);
	QuadTree(LineSet* map, PointSet* points);
	QuadTree(PointSet* points);
	~QuadTree();
	Point* insert(const Point* point);
	inline bool isInside(const Point* point){ return range.isInside(point->x, point->y); }
	inline bool hasPointInTri(Triangle* triangle);
	void pointInTri(Triangle* triangle, vector<Point*>* record);
	bool remove(const Point* point);
};

#endif