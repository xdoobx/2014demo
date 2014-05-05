
#include "stdafx.h"
#include "QuadTree.h"

QuadTree::QuadTree(Rect rect):Index()
{
	range = rect;
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
}

QuadTree::QuadTree(LineSet* map) :Index()
{
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 1; j < map->lines[i].points.size() - 1; ++j){
			insert(&map->lines[i].points[j]);
		}
	}
}

QuadTree::QuadTree(LineSet* map, int mark) :Index()
{
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < map->lines.size(); ++i){
		insert(&map->lines[i].points[0]);
		insert(&map->lines[i].points[map->lines[i].points.size() - 1]);
	}
}

QuadTree::QuadTree(LineSet* map, PointSet* points) :Index()
{
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < points->points.size(); ++i)
		insert(&points->points[i]);
	for (int i = 0; i < map->lines.size(); ++i){
		insert(&map->lines[i].points[0]);
		insert(&map->lines[i].points[map->lines[i].points.size() - 1]);
	}
}
QuadTree::QuadTree(PointSet* points) :Index()
{
	range = Rect(points->minX - 0.1, points->maxX + 0.1, points->minY - 0.1, points->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < points->points.size(); ++i)
		insert(&points->points[i]);
}

QuadTree::~QuadTree(){
	if (subTree[0] != NULL){
		delete subTree[0];
		delete subTree[1];
		delete subTree[2];
		delete subTree[3];
	}
}

bool QuadTree::insert(const Point* newPoint){
	if (!isInside(newPoint))
		return false;
	if (size == 0){
		point = newPoint;
		size = 1;
		return true;
	}
	else if (point != NULL && point->x == newPoint->x && point->y == newPoint->y){
		return false;
	}
	if (subTree[0] == NULL)
		subDivid();
	if (subTree[0]->insert(newPoint) || subTree[1]->insert(newPoint) ||
		subTree[2]->insert(newPoint) || subTree[3]->insert(newPoint)){
		++size;
		return true;
	}
	return false;
}

bool QuadTree::subDivid(){
	if (subTree[0] != NULL || subTree[1] != NULL || subTree[2] != NULL || subTree[3] != NULL)
		return false;
	double halfW = (range.maxX - range.minX) / 2;
	double halfH = (range.maxY - range.minY) / 2;
	subTree[0] = new QuadTree(Rect(range.minX, range.minX + halfW, range.minY, range.minY + halfH));
	subTree[1] = new QuadTree(Rect(range.minX, range.minX + halfW, range.minY + halfH, range.maxY));
	subTree[2] = new QuadTree(Rect(range.minX + halfW, range.maxX, range.minY + halfH, range.maxY));
	subTree[3] = new QuadTree(Rect(range.minX + halfW, range.maxX, range.minY, range.minY + halfH));
	if (subTree[0]->insert(point) || subTree[1]->insert(point) ||
		subTree[2]->insert(point) || subTree[3]->insert(point)){
		point = NULL;
		return true;
	}
	return false;
}

inline bool QuadTree::isInTri(Triangle* triangle, double x, double y){
	double prod1 = (x - triangle->p[1]->x)*(triangle->p[0]->y - y) -
		(x - triangle->p[0]->x)*(triangle->p[1]->y - y);
	double prod2 = (x - triangle->p[2]->x)*(triangle->p[0]->y - y) -
		(x - triangle->p[0]->x)*(triangle->p[2]->y - y);
	double prod3 = (x - triangle->p[2]->x)*(triangle->p[1]->y - y) -
		(x - triangle->p[1]->x)*(triangle->p[2]->y - y);

	return (prod1 > 0) != (prod2 > 0) && (prod2 > 0) != (prod3 > 0) && (prod1 != 0 || prod3 != 0);
}

bool QuadTree::remove(const Point* newPoint){
	if (!isInside(newPoint) || size == 0)
		return false;

	if (subTree[0] != NULL){
		if (subTree[0]->remove(newPoint) || subTree[1]->remove(newPoint) ||
			subTree[2]->remove(newPoint) || subTree[3]->remove(newPoint)){
			--size;
			if (size == 1){
				if (subTree[0]->point != NULL)
					point = subTree[0]->point;
				else if (subTree[1]->point != NULL)
					point = subTree[1]->point;
				else if (subTree[2]->point != NULL)
					point = subTree[2]->point;
				else if (subTree[3]->point != NULL)
					point = subTree[3]->point;
				delete subTree[0];
				subTree[0] = NULL;
				delete subTree[1];
				subTree[1] = NULL;
				delete subTree[2];
				subTree[2] = NULL;
				delete subTree[3];
				subTree[3] = NULL;
			}
			return true;
		}
		else return false;
	}
	else{
		point = NULL;
		size = 0;
		return true;
	}
}

void QuadTree::isDup(const Point* Point, vector<int>* dup){
	if (!isInside(Point) || size == 0)
		return;
	if (point != NULL){
		if (point->x == Point->x && point->y == Point->y)
			dup->push_back(point->lineInd);
		return;
	}
	subTree[0]->isDup(Point, dup);
	subTree[1]->isDup(Point, dup);
	subTree[2]->isDup(Point, dup);
	subTree[3]->isDup(Point, dup);
}

inline bool QuadTree::hasPointInTri(Triangle* triangle){
	if (size == 0)
		return false;
	else if (size == 1)
		return isInTri(triangle, point->x, point->y);
	else if ((triangle->maxX() < range.minX || triangle->minX() > range.maxX ||
		triangle->maxY() < range.minY || triangle->minY() > range.maxY) && // out of range
		!isInside(triangle->p[0]) && !isInside(triangle->p[1]) && !isInside(triangle->p[2]) && // triangle point in rec
		!isInTri(triangle, range.maxX, range.maxY) && !isInTri(triangle, range.minX, range.minY) &&
		!isInTri(triangle, range.maxX, range.minY) && !isInTri(triangle, range.minX, range.maxY)) // rec point in triangle
		//outside each other
		return false;
	else{ //intersect and have more than 1 points
		return subTree[0]->hasPointInTri(triangle) || subTree[1]->hasPointInTri(triangle) ||
			subTree[2]->hasPointInTri(triangle) || subTree[3]->hasPointInTri(triangle);
	}
}

 void QuadTree::pointInTri(Triangle* triangle, vector<Point*>* v){
	if (size == 0)
		return;
	else if (size == 1){
		if (point->kept && isInTri(triangle, point->x, point->y))
			v->push_back((Point*)point);
		return;
	}
	else if ((triangle->maxX() < range.minX || triangle->minX() > range.maxX ||
		triangle->maxY() < range.minY || triangle->minY() > range.maxY) && // out of range
		!isInside(triangle->p[0]) && !isInside(triangle->p[1]) && !isInside(triangle->p[2]) && // triangle point in rec
		!isInTri(triangle, range.maxX, range.maxY) && !isInTri(triangle, range.minX, range.minY) &&
		!isInTri(triangle, range.maxX, range.minY) && !isInTri(triangle, range.minX, range.maxY)){ // rec point in triangle
		//outside each other
		return;
	}
	else{ //intersect and have more than 1 points
		subTree[0]->pointInTri(triangle, v);
		subTree[1]->pointInTri(triangle, v);
		subTree[2]->pointInTri(triangle, v);
		subTree[3]->pointInTri(triangle, v);
	}

}