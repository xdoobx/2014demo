#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "QuadTree.h"

class KDTree{
private:
	inline bool subDivid();
	inline bool isCross(const Point* p1, const Point* p2,
		const double& p3x, const double& p3y, const double& p4x, const double& p4y);
	inline bool isIntersect(const Triangle* triangle);
public:
	Rect range; //range of current branch node
	int size; // number of points under the node

	Point* point; //if it is a leaf node, it should record a point, otherwise null
	KDTree* subTree1; //any index should guarantee they have 4 subtree under root node
	KDTree* subTree2;

	KDTree(const double& minX, const double& maxX, const double& minY, const double& maxY);//construct empty qtree
	KDTree(LineSet* map); //construct qtree with inner points in a map
	KDTree(LineSet* map, int mark); //construct qtree with endpoints in a map
	KDTree(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points
	KDTree(PointSet* points); //construct qtree with constraint points
	//~QuadTree();
	inline const Point* insert(Point* point);
	virtual inline bool hasPointInTri(const Triangle* triangle);
	virtual inline void pointInTri(const Triangle* triangle, vector<Point*>* v);
	virtual inline bool remove(const Point* point);
};

#endif