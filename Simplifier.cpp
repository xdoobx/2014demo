
#include "stdafx.h"
#include "Simplifier.h"

Simplifier::Simplifier(char* lineFile, char* pointFile){
	//clock_t begin = clock(), end;
	LineSet* Map = readLines(lineFile);
	PointSet* Points = readPoints(pointFile);
	qTreeLine = new QuadTree(Map);
	qTreePoint = new QuadTree(Map, Points);
	map = Map;
	points = Points;
	orig_size = qTreeLine->size;
//	end = clock();
//	cout << "construction: " << end - begin << endl;
//	cout << "total line points: " << qTreeLine->size<< endl;
//	cout << "total constriant points: " << qTreePoint->size << endl;
}

//Simplifier::~Simplifier(){
//	delete map;
//	delete points;
//	delete qTreeLine;
//	delete qTreePoint;
//	//delete qTreeEnd;
//}

void Simplifier::wirteFile(string writeFile) {
	//clock_t begin = clock();
	writeLines(map, writeFile);
	//cout << "write into file: " << clock() - begin << endl;
}

inline bool Simplifier::removeP(Triangle &triangle, int index){
	if (!qTreePoint->hasPointInTri(&triangle)){ //is there any city in this triangle?
		if (!qTreeLine->hasPointInTri(&triangle)){ //is there any points on lines in this triangle?
			if (map->lines[triangle.p[index]->lineInd]->kept == 3){ //is it the last point on the line?
				for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd->size(); ++i){
					if (map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd->at(i)]->kept == 2)
						return false; //Oops, sharing both endpoints with a segment
				}
			}
			--map->lines[triangle.p[index]->lineInd]->kept; //line point number --
			triangle.p[index]->kept = false; //set point to be removed
			qTreeLine->remove(triangle.p[index]); //remove point from index
			triangle.p[index] = triangle.p[(index + 2) % 3]; //set triangle ready for next point
			return true;
		}
		else{ //try to remove restricting line points first
			vector<Point*>* constraint = new vector<Point*>;
			qTreeLine->pointInTri(&triangle, constraint);
			Triangle tri;
			Point* toRm;
			int pre_ind, next_ind;
			bool success = true;
			for (int i = 0; i < constraint->size(); ++i){ //remove each point
				toRm = constraint->at(i);
				pre_ind = toRm->pointInd - 1;
				next_ind = toRm->pointInd + 1;
				while (map->lines[toRm->lineInd]->points[pre_ind]->kept != true)
					--pre_ind;
				while (map->lines[toRm->lineInd]->points[next_ind]->kept != true)
					++next_ind;
				tri.p[0] = map->lines[toRm->lineInd]->points[pre_ind];
				tri.p[1] = toRm;
				tri.p[2] = map->lines[toRm->lineInd]->points[next_ind];
				tri.sortX();
				tri.sortY();
				if (!removeP(tri, 1)){
					success = false;
					break;
				}
			}
			//delete constraint;
			if (success){
				if (map->lines[triangle.p[index]->lineInd]->kept == 3){
					for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd->size(); ++i){
						if (map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd->at(i)]->kept == 2)
							return false;
					}
				}
				--map->lines[triangle.p[index]->lineInd]->kept;
				triangle.p[index]->kept = false;
				qTreeLine->remove(triangle.p[index]);
				triangle.p[index] = triangle.p[(index + 2) % 3];
				return true;
			}
		}
	}
	return false;
}

void Simplifier::simplify(int limit){
	Triangle triangle;
	int current;
	int next;
	int last;
	int removed = 1, total_removed = 0;
	//clock_t begin, end;
	while(total_removed < limit && removed != 0){ //repeat the process if more points need to be removed
		//begin = clock();
		for (int i = 0; i < map->lines.size(); ++i){ //for each line
			triangle.p[0] = map->lines[i]->points[0];
			next = 1;
			current = 1;
			while (!map->lines[i]->points[next]->kept) // get first effective triangle
				++next;
			if (map->lines[i]->points[next] == map->lines[i]->points[map->lines[i]->points.size() - 1])
				continue; // it is already a segment
			triangle.p[1] = map->lines[i]->points[next];
			last = map->lines[i]->points.size() - 1;
			if (*(triangle.p[0]) == map->lines[i]->points[last]){ //if the line is a cycle
				for (int j = next; j < last; ++j){
					if (map->lines[i]->kept == 4) //leave at least 4 points
						break;
					while (j < last && !map->lines[i]->points[j + 1]->kept)
						++j;
					triangle.p[(current + 1) % 3] = map->lines[i]->points[j + 1];
					triangle.sortX();
					triangle.sortY();
					removeP(triangle, current % 3);
					++current;
				}
			}
			else{
				for (int j = next; j < last; ++j){
					while (j < last && !map->lines[i]->points[j + 1]->kept) // find next point to remove
						++j;
					triangle.p[(current + 1) % 3] = map->lines[i]->points[j + 1];
					triangle.sortX();
					triangle.sortY();
					removeP(triangle, current % 3); //can I remove the current point?
					++current;
				}
			}
		}
		//end = clock();
		removed = orig_size - qTreeLine->size;
		total_removed += removed;
		orig_size = qTreeLine->size;
		//cout << "remove points: " << removed << "\ttime cost: " << end - begin << endl;
	}
}