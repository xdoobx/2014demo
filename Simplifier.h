#ifndef _SIMPLIFIER_H_
#define _SIMPLIFIER_H_

#include "QuadTree.h"
#include <thread>

class Simplifier{
private:
	Index* qTreeLine; //index removable points
	Index* qTreePoint; //index static points (constraint points + line endpoints)
	LineSet* map; //store lines
	PointSet* points; //store constraint points
	int orig_size; //number of unique points in line set
	inline bool removeP(Triangle &triangle); //remove a point in the given triangle
	inline bool removeS(Triangle &triangle); //simple version of remove point
public:
	Simplifier(char* lineFile, char* pointFile);
	//~Simplifier();
	void simplify(int limit); //main process!
	void simplifyT(Index* &root, const Rect& rect, Triangle& tri); //try to use multi-thread
	void simplifyMT(int limit); //main precess for multi-thread
	void wirteFile(string writeFile);
};

#endif