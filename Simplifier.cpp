
#include "stdafx.h"
#include "Simplifier.h"

LineSet* indexView(QuadTree* tree, LineSet* prev, int &id){
	Line* line = new Line;
	line->id = to_string(id);
	line->points.push_back(Point(tree->range.maxX, tree->range.maxY));
	line->points.push_back(Point(tree->range.maxX, tree->range.minY));
	line->points.push_back(Point(tree->range.minX, tree->range.minY));
	line->points.push_back(Point(tree->range.minX, tree->range.maxY));
	line->points.push_back(Point(tree->range.maxX, tree->range.maxY));
	prev->lines.push_back(line);
	if (tree->subTree[0] != NULL){
		prev = indexView(tree->subTree[0], prev, ++id);
		prev = indexView(tree->subTree[1], prev, ++id);
		prev = indexView(tree->subTree[2], prev, ++id);
		prev = indexView(tree->subTree[3], prev, ++id);
	}
	return prev;
}

Simplifier::Simplifier(char* lineFile, char* pointFile){
	clock_t begin = clock(), end;
	LineSet* Map = readLines(lineFile);
	PointSet* Points = readPoints(pointFile);
	qTreeLine = new QuadTree(Map);
	//qTreeEnd = new QuadTree(Map, 0);
	qTreePoint = new QuadTree(Map, Points);
	map = Map;
	points = Points;
	orig_size = qTreeLine->size;
	end = clock();
	cout << "construction: " << end - begin << endl;
	cout << "total line points: " << qTreeLine->size<< endl;
	cout << "total constriant points: " << qTreePoint->size << endl;
}

Simplifier::~Simplifier(){
	delete map;
	delete points;
	delete qTreeLine;
	delete qTreePoint;
	//delete qTreeEnd;
}

void Simplifier::wirteFile(string writeFile) {
	clock_t begin = clock();
	writeLines(map, writeFile);
	cout << "write into file: " << clock() - begin << endl;
}

inline bool Simplifier::removeP(Triangle &triangle, int index){
	if (!qTreePoint->hasPointInTri(&triangle)){
		if (!qTreeLine->hasPointInTri(&triangle)){
			if (map->lines[triangle.p[index]->lineInd]->kept == 3){
				for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd11->size(); ++i){
					for (int j = 0; j < map->lines[triangle.p[index]->lineInd]->shareEnd22->size(); ++j){
						if (map->lines[triangle.p[index]->lineInd]->shareEnd11->at(i) ==
							map->lines[triangle.p[index]->lineInd]->shareEnd22->at(j) &&
							map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd11->at(i)]->kept == 2)
							return false;
					}
				}
				for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd12->size(); ++i){
					for (int j = 0; j < map->lines[triangle.p[index]->lineInd]->shareEnd21->size(); ++j){
						if (map->lines[triangle.p[index]->lineInd]->shareEnd12->at(i) ==
							map->lines[triangle.p[index]->lineInd]->shareEnd21->at(j) &&
							map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd12->at(i)]->kept == 2)
							return false;
					}
				}
			}
			--map->lines[triangle.p[index]->lineInd]->kept;
			triangle.p[index]->kept = false;
			qTreeLine->remove(triangle.p[index]);
			triangle.p[index] = triangle.p[(index + 2) % 3];
			return true;
		}
		else{
			vector<Point*>* constraint = new vector<Point*>;
			qTreeLine->pointInTri(&triangle, constraint);
			Triangle tri;
			Point* toRm;
			int pre_ind, next_ind;
			bool success = true;
			for (int i = 0; i < constraint->size(); ++i){
				toRm = constraint->at(i);
				pre_ind = toRm->pointInd - 1;
				next_ind = toRm->pointInd + 1;
				while (map->lines[toRm->lineInd]->points[pre_ind].kept != true)
					--pre_ind;
				while (map->lines[toRm->lineInd]->points[next_ind].kept != true)
					++next_ind;
				tri.p[0] = &map->lines[toRm->lineInd]->points[pre_ind];
				tri.p[1] = toRm;
				tri.p[2] = &map->lines[toRm->lineInd]->points[next_ind];
				if (!removeP(tri, 1)){
					success = false;
					break;
				}
			}
			delete constraint;
			if (success){
				if (map->lines[triangle.p[index]->lineInd]->kept == 3){
					for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd11->size(); ++i){
						for (int j = 0; j < map->lines[triangle.p[index]->lineInd]->shareEnd22->size(); ++j){
							if (map->lines[triangle.p[index]->lineInd]->shareEnd11->at(i) ==
								map->lines[triangle.p[index]->lineInd]->shareEnd22->at(j) &&
								map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd11->at(i)]->kept == 2)
								return false;
						}
					}
					for (int i = 0; i < map->lines[triangle.p[index]->lineInd]->shareEnd12->size(); ++i){
						for (int j = 0; j < map->lines[triangle.p[index]->lineInd]->shareEnd21->size(); ++j){
							if (map->lines[triangle.p[index]->lineInd]->shareEnd12->at(i) ==
								map->lines[triangle.p[index]->lineInd]->shareEnd21->at(j) &&
								map->lines[map->lines[triangle.p[index]->lineInd]->shareEnd12->at(i)]->kept == 2)
								return false;
						}
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

void Simplifier::simplify(){
	Triangle triangle;
	int current;
	int next;
	int last;
	int removed;
	clock_t begin, end;
	do{
		begin = clock();
		for (int i = 0; i < map->lines.size(); ++i){
			triangle.p[0] = &map->lines[i]->points[0];
			next = 1;
			current = 1;
			while (!map->lines[i]->points[next].kept)
				++next;
			if (map->lines[i]->points[next] == map->lines[i]->points[map->lines[i]->points.size() - 1])
				continue;
			triangle.p[1] = &map->lines[i]->points[next];
			last = map->lines[i]->points.size() - 1;
			if (*(triangle.p[0]) == &map->lines[i]->points[last]){
				for (int j = next; j < last; ++j){
					if (map->lines[i]->kept == 4)
						break;
					while (j < last && !map->lines[i]->points[j + 1].kept)
						++j;
					triangle.p[(current + 1) % 3] = &map->lines[i]->points[j + 1];
					removeP(triangle, current % 3);
					++current;
				}
			}
			else{
				for (int j = next; j < last; ++j){
					while (j < last && !map->lines[i]->points[j + 1].kept)
						++j;
					triangle.p[(current + 1) % 3] = &map->lines[i]->points[j + 1];
					removeP(triangle, current % 3);
					++current;
				}
			}
		}
		end = clock();
		removed = orig_size - qTreeLine->size;
		orig_size = qTreeLine->size;
		cout << "remove points: " << removed << "\ttime cost: " << end - begin << endl;
	} while (removed != 0);
	//int id = 1000;
	//indexView(qTreeLine, map, id);
}