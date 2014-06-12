#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "QuadTree.h"
#include <thread>

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

	//vector<vector<Point*>*>** grid_Multiple;
	//bool emptyFlag[100][100];//mark if the grid is empty.
	GridTree(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points

	inline const Point* insert(Point* point);
	//inline const Point* insertM(Point* point);
	virtual inline bool hasPointInTri(const Triangle* triangle);
};

#endif