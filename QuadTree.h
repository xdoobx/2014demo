#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Index.h"
#include "FileIO.h"

class QuadTree: private Index
{
private:
	bool subDivid();
	inline bool isCross(const Point* p1, const Point* p2,
		const double& p3x, const double& p3y, const double& p4x, const double& p4y);
	inline bool isIntersect(const Triangle* triangle);
public:
	Rect range;
	int size;
	const Point* point;

	QuadTree* subTree[4];

	QuadTree(const double& minX, const double& maxX, const double& minY, const double& maxY);//construct empty qtree
	QuadTree(LineSet* map); //construct qtree with inner points in a map
	QuadTree(LineSet* map, int mark); //construct qtree with endpoints in a map
	QuadTree(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points
	QuadTree(PointSet* points); //construct qtree with constraint points
	//~QuadTree();
	Point* insert(const Point* point);
	inline bool isInside(const Point* point){ return range.isInside(point->x, point->y); }
	inline bool hasPointInTri(const Triangle* triangle);
	void pointInTri(const Triangle* triangle, vector<Point*>* record);
	bool remove(const Point* point);
};

#endif