
#include "stdafx.h"
#include "QuadTree.h"

QuadTree::QuadTree(const double& minX, const double& maxX, const double& minY, const double& maxY) :Index()
{
	range.minX = minX;
	range.maxX = maxX;
	range.minY = minY;
	range.maxY = maxY;
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
}

QuadTree::QuadTree(LineSet* map) :Index()
{
	clock_t begin = clock();
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 1; j < map->lines[i]->points.size() - 1; ++j){
			insert(map->lines[i]->points[j]);
		}
	}
	cout << "construct line index cost: " << clock() - begin << endl;
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
		insert(map->lines[i]->points[0]);
		insert(map->lines[i]->points[map->lines[i]->points.size() - 1]);
	}
}

QuadTree::QuadTree(LineSet* map, PointSet* points) :Index()
{
	clock_t begin = clock();
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree[0] = NULL;
	subTree[1] = NULL;
	subTree[2] = NULL;
	subTree[3] = NULL;
	for (int i = 0; i < points->points.size(); ++i)
		insert(points->points[i]);
	Point* temp;
	vector<int>* dup1, *dup2;
	for (int i = 0; i < map->lines.size(); ++i){ // find lines sharing endpoints
		temp = insert(map->lines[i]->points[0]); //try to insert head point
		if (temp != map->lines[i]->points[0] && temp->lineInd != i){ //a mate is already there
			if (temp->pointInd == 0){ //if it is a head point
				dup1 = map->lines[temp->lineInd]->shareEnd11; //who's sharing endpoint with the head
				dup2 = map->lines[temp->lineInd]->shareEnd12;
				for (int j = 0; j < dup1->size(); ++j){ //tell other head mates I'm here, also record them
					map->lines[dup1->at(j)]->shareEnd11->push_back(i);
					map->lines[i]->shareEnd11->push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){ //do the same to the tail mates
					map->lines[dup2->at(j)]->shareEnd21->push_back(i);
					map->lines[i]->shareEnd12->push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd11->push_back(i); //do it to the original mate
				map->lines[i]->shareEnd11->push_back(temp->lineInd);
			}
			else{ //or the mate is a tail
				dup1 = map->lines[temp->lineInd]->shareEnd21; // who's sharing ep with the tail
				dup2 = map->lines[temp->lineInd]->shareEnd22;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd11->push_back(i);
					map->lines[i]->shareEnd11->push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd21->push_back(i);
					map->lines[i]->shareEnd12->push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd21->push_back(i);
				map->lines[i]->shareEnd12->push_back(temp->lineInd);
			}
		}
		temp = insert(map->lines[i]->points[map->lines[i]->points.size() - 1]); //try to insert tail
		if (temp != map->lines[i]->points[map->lines[i]->points.size() - 1]){
			if (temp->pointInd == 0){
				dup1 = map->lines[temp->lineInd]->shareEnd11;
				dup2 = map->lines[temp->lineInd]->shareEnd12;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd12->push_back(i);
					map->lines[i]->shareEnd21->push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd22->push_back(i);
					map->lines[i]->shareEnd22->push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd12->push_back(i);
				map->lines[i]->shareEnd21->push_back(temp->lineInd);
			}
			else{
				dup1 = map->lines[temp->lineInd]->shareEnd21;
				dup2 = map->lines[temp->lineInd]->shareEnd22;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd12->push_back(i);
					map->lines[i]->shareEnd21->push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd22->push_back(i);
					map->lines[i]->shareEnd22->push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd22->push_back(i);
				map->lines[i]->shareEnd22->push_back(temp->lineInd);
			}
		}
	}
	map->getShare(); // find the lines sharing both endpoints
	cout << "construct point index cost: " << clock() - begin << endl;
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
		insert(points->points[i]);
}

//QuadTree::~QuadTree(){
//	if (subtree[0] != null){
//		delete subtree[0];
//		delete subtree[1];
//		delete subtree[2];
//		delete subtree[3];
//	}
//}

Point* QuadTree::insert(const Point* newPoint){
	if (!isInside(newPoint))
		return NULL;
	if (size == 0){
		point = newPoint;
		size = 1;
		return (Point*)newPoint;
	}
	if (point != NULL && *point == newPoint){ //return the existing point if conflicts
		return (Point*)point;
	}
	if (subTree[0] == NULL)
		subDivid();
	Point* p1 = subTree[0]->insert(newPoint);
	Point* p2 = subTree[1]->insert(newPoint);
	Point* p3 = subTree[2]->insert(newPoint);
	Point* p4 = subTree[3]->insert(newPoint);
	if (p1 == newPoint || p2 == newPoint ||
		p3 == newPoint || p4 == newPoint){ //insert successfully
		++size;
		return (Point*)newPoint;
	}
	else if (p1 != NULL) //return the existing point out
		return p1;
	else if (p2 != NULL)
		return p2;
	else if (p3 != NULL)
		return p3;
	else if (p4 != NULL)
		return p4;
	return NULL;
}

bool QuadTree::subDivid(){
	if (subTree[0] != NULL || subTree[1] != NULL || subTree[2] != NULL || subTree[3] != NULL)
		return false;
	double halfW = (range.maxX - range.minX) / 2; //divide original area into 4
	double halfH = (range.maxY - range.minY) / 2;
	subTree[0] = new QuadTree(range.minX, range.minX + halfW, range.minY, range.minY + halfH);
	subTree[1] = new QuadTree(range.minX, range.minX + halfW, range.minY + halfH, range.maxY);
	subTree[2] = new QuadTree(range.minX + halfW, range.maxX, range.minY + halfH, range.maxY);
	subTree[3] = new QuadTree(range.minX + halfW, range.maxX, range.minY, range.minY + halfH);
	if (subTree[0]->insert(point) || subTree[1]->insert(point) ||
		subTree[2]->insert(point) || subTree[3]->insert(point)){
		point = NULL; //put point into lower layer, empty itself
		return true;
	}
	return false;
}

inline bool QuadTree::isCross(const Point* p1, const Point* p2,
	const double& p3x, const double& p3y, const double& p4x, const double& p4y){
	double prod1 = (p3x - p1->x)*(p2->y - p3y) - (p3x - p2->x)*(p1->y - p3y);
	double prod2 = (p4x - p1->x)*(p2->y - p4y) - (p4x - p2->x)*(p1->y - p4y);
	if ((prod1 > 0) == (prod2 > 0) || prod1 == 0 || prod2 == 0) // p1p2 divide p3p4
		return false;

	prod1 = (p1->x - p3x)*(p4y - p1->y) - (p1->x - p4x)*(p3y - p1->y);
	prod2 = (p2->x - p3x)*(p4y - p2->y) - (p2->x - p4x)*(p3y - p2->y);
	if ((prod1 > 0) == (prod2 > 0) || prod1 == 0 || prod2 == 0) //p3p4 divide p1p2
		return false;

	return true;
}

inline bool QuadTree::isIntersect(const Triangle* triangle){ //triangle * rectangle = 12
	return isCross(triangle->p[0], triangle->p[1], range.minX, range.minY, range.maxX, range.minY) ||
		isCross(triangle->p[0], triangle->p[2], range.minX, range.minY, range.maxX, range.minY) ||
		isCross(triangle->p[2], triangle->p[1], range.minX, range.minY, range.maxX, range.minY) ||
		isCross(triangle->p[0], triangle->p[1], range.maxX, range.minY, range.maxX, range.maxY) ||
		isCross(triangle->p[0], triangle->p[2], range.maxX, range.minY, range.maxX, range.maxY) ||
		isCross(triangle->p[2], triangle->p[1], range.maxX, range.minY, range.maxX, range.maxY) ||
		isCross(triangle->p[0], triangle->p[1], range.maxX, range.maxY, range.minX, range.maxY) ||
		isCross(triangle->p[0], triangle->p[2], range.maxX, range.maxY, range.minX, range.maxY) ||
		isCross(triangle->p[2], triangle->p[1], range.maxX, range.maxY, range.minX, range.maxY) ||
		isCross(triangle->p[0], triangle->p[1], range.minX, range.maxY, range.minX, range.minY) ||
		isCross(triangle->p[0], triangle->p[2], range.minX, range.maxY, range.minX, range.minY) ||
		isCross(triangle->p[2], triangle->p[1], range.minX, range.maxY, range.minX, range.minY);
}

bool QuadTree::remove(const Point* newPoint){
	if (!isInside(newPoint) || size == 0) //out of range
		return false;

	if (subTree[0] != NULL){ //have children
		if (subTree[0]->remove(newPoint) || subTree[1]->remove(newPoint) ||
			subTree[2]->remove(newPoint) || subTree[3]->remove(newPoint)){
			--size; //remove successfully
			if (size == 1){ //need to recycle empty branches
				if (subTree[0]->point != NULL)
					point = subTree[0]->point;
				else if (subTree[1]->point != NULL)
					point = subTree[1]->point;
				else if (subTree[2]->point != NULL)
					point = subTree[2]->point;
				else if (subTree[3]->point != NULL)
					point = subTree[3]->point;
				//delete subTree[0];
				subTree[0] = NULL;
				//delete subTree[1];
				subTree[1] = NULL;
				//delete subTree[2];
				subTree[2] = NULL;
				//delete subTree[3];
				subTree[3] = NULL;
			}
			return true;
		}
	}
	else{
		point = NULL;
		size = 0;
		return true;
	}
	return false;
}

inline bool QuadTree::hasPointInTri(const Triangle* triangle){
	if (size == 0)
		return false;
	else if (size == 1)
		return triangle->isInTri(point->x, point->y);
	else if (triangle->maxX < range.minX || triangle->minX > range.maxX ||
		triangle->maxY < range.minY || triangle->minY > range.maxY) // out of range
		return false;
	else if(!isInside(triangle->p[0]) && !isInside(triangle->p[1]) && !isInside(triangle->p[2]) && // triangle point in rec
		!triangle->isInTri(range.maxX, range.maxY) && !triangle->isInTri(range.minX, range.minY) &&
		!triangle->isInTri(range.maxX, range.minY) && !triangle->isInTri(range.minX, range.maxY) && // rec point in triangle
		!isIntersect(triangle)) //outside each other
		return false;
	else{ //intersect and have more than 1 points
		return subTree[0]->hasPointInTri(triangle) || subTree[1]->hasPointInTri(triangle) ||
			subTree[2]->hasPointInTri(triangle) || subTree[3]->hasPointInTri(triangle);
	}
}

void QuadTree::pointInTri(const Triangle* triangle, vector<Point*>* v){
	if (size == 0)
		return;
	else if (size == 1){
		if (point->kept && triangle->isInTri(point->x, point->y))
			v->push_back((Point*)point);
		return;
	}
	else if (triangle->maxX < range.minX || triangle->minX > range.maxX ||
		triangle->maxY < range.minY || triangle->minY > range.maxY) // out of range
		return;
	else if(!isInside(triangle->p[0]) && !isInside(triangle->p[1]) && !isInside(triangle->p[2]) && // triangle point in rec
		!triangle->isInTri(range.maxX, range.maxY) && !triangle->isInTri(range.minX, range.minY) &&
		!triangle->isInTri(range.maxX, range.minY) && !triangle->isInTri(range.minX, range.maxY) && // rec point in triangle
		!isIntersect(triangle)){ //their segments do not intersect
		return;
	}
	else{ //intersect and have more than 1 points
		subTree[0]->pointInTri(triangle, v);
		subTree[1]->pointInTri(triangle, v);
		subTree[2]->pointInTri(triangle, v);
		subTree[3]->pointInTri(triangle, v);
	}
}