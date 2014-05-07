#ifndef _SIMPLIFIER_H_
#define _SIMPLIFIER_H_

#include "QuadTree.h"

class Simplifier{
private:
	QuadTree* qTreeLine; //index removable points
	QuadTree* qTreePoint; //index static points (constraint points + line endpoints)
	LineSet* map; //store lines
	PointSet* points; //store constraint points
	int orig_size; //nnumber of unique points in line set
	inline bool removeP(Triangle &triangle, int index); //remove a point in the given triangle
public:
	Simplifier(char* lineFile, char* pointFile);
	//~Simplifier();
	void simplify(int limit); //main process!
	void wirteFile(string writeFile);
};

#endif