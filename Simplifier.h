#ifndef _SIMPLIFIER_H_
#define _SIMPLIFIER_H_

#include "FileIO.h"
#include "QuadTree.h"

class Simplifier{
private:
	QuadTree* qTreeLine;
	QuadTree* qTreePoint;
	//QuadTree* qTreeEnd;
	LineSet* map;
	PointSet* points;
	int orig_size;
	inline bool removeP(Triangle &triangle, int index);
public:
	Simplifier(char* lineFile, char* pointFile);
	~Simplifier();
	void simplify();
	LineSet* getMap()const{ return map; }
	void wirteFile(string writeFile);
};

#endif