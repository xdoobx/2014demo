
#include "stdafx.h"
#include "SortIndex.h"

void SortIndex::merge(vector<Point*> &arr, int start, int length, vector<Point*> &temp, bool byX){
	int period = length / 2;
	int p1 = start, mid = start + period, p2 = mid, end = start + length;
	if (byX){
		for (int i = 0; i < length; ++i){ // find the smallest number among the three headers
			if (p1 < mid && (p2 >= end || arr[p1]->x <= arr[p2]->x)){
				temp[start + i] = arr[p1];
				++p1;
			}
			else{
				temp[start + i] = arr[p2];
				++p2;
			}
		}
	}
	else{
		for (int i = 0; i < length; ++i){ // find the smallest number among the three headers
			if (p1 < mid && (p2 >= end || arr[p1]->y <= arr[p2]->y)){
				temp[start + i] = arr[p1];
				++p1;
			}
			else{
				temp[start + i] = arr[p2];
				++p2;
			}
		}
	}
	for (int i = 0; i < length; ++i){ // over writer the array from temp array
		arr[start + i] = temp[start + i];
	}
}

void SortIndex::mergeSort(vector<Point*> &arr, int start, int length, vector<Point*> &temp, bool byX){
	if (length < 2) // atomic sub-array
		return;
	else if (length == 2){ // trivial sub-array
		if (byX){
			if (arr[start]->x > arr[start + 1]->x){
				temp[start] = arr[start];
				arr[start] = arr[start + 1];
				arr[start + 1] = temp[start];
			}
		}
		else{
			if (arr[start]->y > arr[start + 1]->y){
				temp[start] = arr[start];
				arr[start] = arr[start + 1];
				arr[start + 1] = temp[start];
			}
		}
	}
	else{
		int period = length / 2;
		mergeSort(arr, start, period, temp, byX);
		mergeSort(arr, start + period, length - period, temp, byX);
		merge(arr, start, length, temp, byX);
	}
}

SortIndex::SortIndex(LineSet* map) :Index(){
	//sortByX.reserve();
	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 1; j < map->lines[i]->points.size() - 1; ++j){
			sortByX.push_back(map->lines[i]->points[j]);
			sortByY.push_back(map->lines[i]->points[j]);
		}
	}
	vector<Point*> temp(sortByX.size());
	mergeSort(sortByX, 0, sortByX.size(), temp, true);
	mergeSort(sortByY, 0, sortByY.size(), temp, false);
}

SortIndex::SortIndex(PointSet* points, LineSet* map) :Index(){
	for (int i = 0; i < points->points.size(); ++i){
		sortByX.push_back(points->points[i]);
		sortByY.push_back(points->points[i]);
	}
	for (int i = 0; i < map->lines.size(); ++i){
		sortByX.push_back(map->lines[i]->points[0]);
		sortByY.push_back(map->lines[i]->points[0]);
		sortByX.push_back(map->lines[i]->points[map->lines[i]->points.size()]);
		sortByY.push_back(map->lines[i]->points[map->lines[i]->points.size()]);
	}
	vector<Point*> temp(sortByX.size());
	mergeSort(sortByX, 0, sortByX.size(), temp, true);
	mergeSort(sortByY, 0, sortByY.size(), temp, false);
}

const Point* SortIndex::insert(Point* newPoint){
	return NULL;
}

bool SortIndex::hasPointInTri(const Triangle* triangle){
	return false;
}

void SortIndex::pointInTri(const Triangle* triangle, vector<Point*>* v){

}

bool SortIndex::remove(const Point* point){
	return true;
}