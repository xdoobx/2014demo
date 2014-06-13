#ifndef _GRIDSIMPLIFIERM_H_
#define _GRIDSIMPLIFIERM_H_

#include "GridTreeM.h"

class GridSimplifierM{
protected:
	GridTreeM* gridIndex; //index all points
	//LineSet* map; //store lines
	LineSetM* map; //store lines

	PointSet* points; //store constraint points
	int orig_size; //number of unique points in line set
	inline bool removeS(Triangle &triangle, int threadId); //simple version of remove point

	const int threadN = 4;//the maximal number of thread, should automatically set this number;
public:
	GridSimplifierM(char* lineFile, char* pointFile);
	//void simplify(int limit); // use single processor
	//void simplifyT(int xl, int xr, int yl, int yr, const Rect& rect, Triangle& tri); // in each thread

	void GridSimplifierM::simplifyT(vector<Line*> lines, Triangle& tri, int threadId);//in each thread

	//void simplifyTL(int index, Triangle& tri);
	void simplifyMT(int limit); //main precess for multi-thread
	void wirteFile(string writeFile);
	void wirteFileM(string writeFile);

};

#endif