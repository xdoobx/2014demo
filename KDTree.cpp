#include "stdafx.h"
#include "KDTree.h"

KDTree::KDTree(const double& minX, const double& maxX, const double& minY, const double& maxY){
	range.minX = minX;
	range.maxX = maxX;
	range.minY = minY;
	range.maxY = maxY;
	size = 0;
	point = NULL;
	subTree1 = NULL;
	subTree2 = NULL;
}

KDTree::KDTree(LineSet* map){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree1 = NULL;
	subTree2 = NULL;
	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 1; j < map->lines[i]->points.size() - 1; ++j){
			insert(map->lines[i]->points[j]);
		}
	}
}

KDTree::KDTree(LineSet* map, int mark){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree1 = NULL;
	subTree2 = NULL;
	for (int i = 0; i < map->lines.size(); ++i){
		insert(map->lines[i]->points[0]);
		insert(map->lines[i]->points[map->lines[i]->points.size() - 1]);
	}
}

KDTree::KDTree(LineSet* map, PointSet* points){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree1 = NULL;
	subTree2 = NULL;
	for (int i = 0; i < points->points.size(); ++i)
		insert(points->points[i]);
	const Point* temp;
	vector<int>* dup1, *dup2;
	for (int i = 0; i < map->lines.size(); ++i){ // find lines sharing endpoints
		temp = insert(map->lines[i]->points[0]); //try to insert head point
		if (temp != map->lines[i]->points[0] && temp->lineInd != i){ //a mate is already there
			if (temp->pointInd == 0){ //if it is a head point
				dup1 = &map->lines[temp->lineInd]->shareEnd11; //who's sharing endpoint with the head
				dup2 = &map->lines[temp->lineInd]->shareEnd12;
				for (int j = 0; j < dup1->size(); ++j){ //tell other head mates I'm here, also record them
					map->lines[dup1->at(j)]->shareEnd11.push_back(i);
					map->lines[i]->shareEnd11.push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){ //do the same to the tail mates
					map->lines[dup2->at(j)]->shareEnd21.push_back(i);
					map->lines[i]->shareEnd12.push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd11.push_back(i); //do it to the original mate
				map->lines[i]->shareEnd11.push_back(temp->lineInd);
			}
			else{ //or the mate is a tail
				dup1 = &map->lines[temp->lineInd]->shareEnd21; // who's sharing ep with the tail
				dup2 = &map->lines[temp->lineInd]->shareEnd22;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd11.push_back(i);
					map->lines[i]->shareEnd11.push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd21.push_back(i);
					map->lines[i]->shareEnd12.push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd21.push_back(i);
				map->lines[i]->shareEnd12.push_back(temp->lineInd);
			}
		}
		temp = insert(map->lines[i]->points[map->lines[i]->points.size() - 1]); //try to insert tail
		if (temp != map->lines[i]->points[map->lines[i]->points.size() - 1]){
			if (temp->pointInd == 0){
				dup1 = &map->lines[temp->lineInd]->shareEnd11;
				dup2 = &map->lines[temp->lineInd]->shareEnd12;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd12.push_back(i);
					map->lines[i]->shareEnd21.push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd22.push_back(i);
					map->lines[i]->shareEnd22.push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd12.push_back(i);
				map->lines[i]->shareEnd21.push_back(temp->lineInd);
			}
			else{
				dup1 = &map->lines[temp->lineInd]->shareEnd21;
				dup2 = &map->lines[temp->lineInd]->shareEnd22;
				for (int j = 0; j < dup1->size(); ++j){
					map->lines[dup1->at(j)]->shareEnd12.push_back(i);
					map->lines[i]->shareEnd21.push_back(dup1->at(j));
				}
				for (int j = 0; j < dup2->size(); ++j){
					map->lines[dup2->at(j)]->shareEnd22.push_back(i);
					map->lines[i]->shareEnd22.push_back(dup2->at(j));
				}
				map->lines[temp->lineInd]->shareEnd22.push_back(i);
				map->lines[i]->shareEnd22.push_back(temp->lineInd);
			}
		}
	}
	map->getShare(); // find the lines sharing both endpoints
}

KDTree::KDTree(PointSet* points){
	range = Rect(points->minX - 0.1, points->maxX + 0.1, points->minY - 0.1, points->maxY + 0.1);
	size = 0;
	point = NULL;
	subTree1 = NULL;
	subTree2 = NULL;
	for (int i = 0; i < points->points.size(); ++i)
		insert(points->points[i]);
}


bool KDTree::isCross(const Point* p1, const Point* p2,
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


bool KDTree::isIntersect(const Triangle* triangle){ //triangle * rectangle = 12
	return
		isCross(triangle->p[0], triangle->p[1], range.minX, range.minY, range.maxX, range.minY) ||
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

const Point* KDTree::insert(Point* newPoint){
	return NULL;
}

bool KDTree::subDivid(){
	return false;
}

bool KDTree::remove(const Point* newPoint){
	return false;
}

bool KDTree::hasPointInTri(const Triangle* triangle){
	return false;
}

void KDTree::pointInTri(const Triangle* triangle, vector<Point*>* v){

}

