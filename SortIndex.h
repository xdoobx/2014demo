#ifndef _SORTINDEX_H_
#define _SORTINDEX_H_

#include "Index.h"
#include "FileIO.h"

class SortIndex :public Index{
private:
	vector<Point*> sortByX;
	vector<Point*> sortByY;
	void merge(vector<Point*> &arr, int start, int length, vector<Point*> &temp, bool byX);
	void mergeSort(vector<Point*> &arr, int start, int length, vector<Point*> &temp, bool byX);
public:
	SortIndex(LineSet* map);
	SortIndex(PointSet* points, LineSet* map);
	const Point* insert(Point* point);
	bool hasPointInTri(const Triangle* triangle);
	void pointInTri(const Triangle* triangle, vector<Point*>* v);
	bool remove(const Point* point);
};


#endif