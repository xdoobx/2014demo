#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "QuadTree.h"

class GridTree{
private:
public:
	Rect range; //range of current branch node
	int size; // number of points under the node
	int divideW = 50;
	double gridW;
	int divideH = 50;
	double gridH;

	vector<Point*>** grid; //store points in this grid
	//GridTree** subTree; //any index should guarantee they have 4 subtree under root node

	GridTree(const double& minX, const double& maxX, const double& minY, const double& maxY);//construct empty qtree
	GridTree(LineSet* map); //construct qtree with inner points in a map
	GridTree(LineSet* map, int mark); //construct qtree with endpoints in a map
	GridTree(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points
	GridTree(PointSet* points); //construct qtree with constraint points
	//~QuadTree();

	inline const Point* insert(Point* point);
	virtual inline bool hasPointInTri(const Triangle* triangle);
	virtual inline void pointInTri(const Triangle* triangle, vector<Point*>* v);
	virtual inline bool remove(const Point* point);
};

#endif