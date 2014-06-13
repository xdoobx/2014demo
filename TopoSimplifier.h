#ifndef _TOPOSIMPLIFIER_H_
#define _TOPOSIMPLIFIER_H_

#include "GridTree.h"
#include <list>



struct Node
{
	Point* vertex;
	double gValue;
	double hValue;
	list<Point*> pointedTo;
	list<Point*> pointedBy;
	list<Point*> pointedBy_Fixeds;
};

class TopoSimplifier{
public:
	LineSet* map; //store lines
	PointSet* points; //store constraint points

	GridTree* gridIndex; //index all points
	vector<Node*> topoGraph;

	int orig_size; //number of unique points in line set
	inline bool removeS(Triangle &triangle); //simple version of remove point

	void buildTopoGraph();
	TopoSimplifier(char* lineFile, char* pointFile);
	void simplify();
	void wirteFile(string writeFile);
};

#endif