#include "stdafx.h"
#include "GridTree.h"

GridTree::GridTree(const double& minX, const double& maxX, const double& minY, const double& maxY){
	range.minX = minX;
	range.maxX = maxX;
	range.minY = minY;
	range.maxY = maxY;
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	grid = new vector<Point*>*[divideW];
	for (int i = 0; i < divideH; ++i)
		grid[i] = new vector<Point*>[divideH];
	size = 0;
}

GridTree::GridTree(LineSet* map){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	size = 0;
	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 1; j < map->lines[i]->points.size() - 1; ++j){
			insert(map->lines[i]->points[j]);
		}
	}
}

GridTree::GridTree(LineSet* map, int mark){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	size = 0;
	for (int i = 0; i < map->lines.size(); ++i){
		insert(map->lines[i]->points[0]);
		insert(map->lines[i]->points[map->lines[i]->points.size() - 1]);
	}
}

GridTree::GridTree(LineSet* map, PointSet* points){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	size = 0;
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

GridTree::GridTree(PointSet* points){
	range = Rect(points->minX - 0.1, points->maxX + 0.1, points->minY - 0.1, points->maxY + 0.1);
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	size = 0;
	for (int i = 0; i < points->points.size(); ++i)
		insert(points->points[i]);
}

const Point* GridTree::insert(Point* newPoint){
	int gridX = (newPoint->x - range.minX) / gridW;
	int gridY = (newPoint->y - range.minY) / gridH;
	grid[gridX][gridY].push_back(newPoint); // even though this point overlays a previous point
	return newPoint;
}

bool GridTree::remove(const Point* newPoint){
	return false;
}

bool GridTree::hasPointInTri(const Triangle* triangle){
	return false;
}

void GridTree::pointInTri(const Triangle* triangle, vector<Point*>* v){

}

