#ifndef _GRIDTREEM_H_
#define _GRIDTREEM_H_

#include "GeoUtility.h"
#include <vector>

class GridTreeM{
private:
public:
	Rect range; //range of current branch node
	int size; // number of points under the node
	int divideW;
	double gridW;
	int divideH;
	double gridH;
	const static int threadN = 4;// the number of useful threads. Here we set a fixed value. But later we should automatically set it based on the CPU.

	vector<Point*> gridM[50][50][threadN+1];
	//vector<Point*> gridM[100][100];

	GridTreeM(LineSet* map, PointSet* points); //construct qtree with endpoints and constraint points

	inline const Point* insertM(Point* point, int tid);
	virtual inline bool hasPointInTri(const Triangle* triangle);
};

#endif