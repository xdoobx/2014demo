
#include "stdafx.h"
#include "GridSimplifierM.h"
#include "FileIO.h"
#include <thread>


GridSimplifierM::GridSimplifierM(char* lineFile, char* pointFile){
	//clock_t begin, end;
	//begin = clock();
	map = readLinesM(lineFile,threadN);
	points = readPoints(pointFile);
	//end = clock();
	//cout << "IO: " << end - begin << endl;

	 //begin = clock();
	gridIndex = new GridTreeM(map, points);
	 //end = clock();
	//cout << "index: " << end - begin << endl;
}



bool GridSimplifierM::removeS(Triangle &triangle, int threadId){
	if (!gridIndex->hasPointInTri(&triangle)){
		triangle.p[1]->kept = false; //set point as removed
		triangle.p[2]->leftInd = triangle.p[0]->pointInd;
		triangle.p[0]->rightInd = triangle.p[2]->pointInd;
		--gridIndex->sizes[threadId]; //remove point from index
		--map->lines[threadId][triangle.p[1]->lineInd]->kept;
		return true;
	}
	else
		return false;
}

bool GridSimplifierM::removeS(Polygon &poly, int threadId){
	if (!gridIndex->hasPointInPoly(&poly)){
		for (int i = 1; i < poly.size; ++i)
			poly.p[i]->kept = false; //set point as removed
		poly.p[poly.size - 1]->leftInd = poly.p[0]->pointInd;
		poly.p[0]->rightInd = poly.p[poly.size - 1]->pointInd;
		gridIndex->sizes[threadId] -= poly.size - 2; //remove point from index
		map->lines[threadId][poly.p[0]->lineInd]->kept -= poly.size - 2;
		return true;
	}
	else
		return false;
}

void GridSimplifierM::simplifyT(vector<Line*> lines, Triangle& tri, int threadId){
	for (int i = 0; i<lines.size(); ++i){
		for (int j = 1; j < lines[i]->points.size() - 1; ++j){
			tri.p[1] = lines[i]->points[j];
			tri.p[0] = lines[tri.p[1]->lineInd]->points[tri.p[1]->leftInd];
			tri.p[2] = lines[tri.p[1]->lineInd]->points[tri.p[1]->rightInd];
			tri.sort();
			removeS(tri,threadId);
		}
	}
}

void GridSimplifierM::simplifyTP(vector<Line*> lines, Polygon& poly, int threadId){
	for (int i = 0; i<lines.size(); ++i){
		for (int j = 1; j < lines[i]->points.size()/2; ++j){
			poly.p[1] = lines[i]->points[j * 2 - 1];
			poly.p[2] = lines[i]->points[j * 2];
			poly.p[0] = lines[i]->points[poly.p[1]->leftInd];
			poly.p[3] = lines[i]->points[poly.p[2]->rightInd];
			poly.getRange();
			removeS(poly, threadId);
		}
		if (lines[i]->points.size() % 2 == 1){
			Triangle tri;
			tri.p[1] = lines[i]->points[lines[i]->points.size()-2];
			tri.p[0] = lines[i]->points[tri.p[1]->leftInd];
			tri.p[2] = lines[i]->points[tri.p[1]->rightInd];
			tri.sort();
			removeS(tri, threadId);
		}
	}
}

void GridSimplifierM::simplifyMT(int limit){
	Triangle tri1;
	Triangle tri2;
	Triangle tri3;
	Triangle tri4;

	thread t0(&GridSimplifierM::simplifyT, this, map->lines[0], tri1, 0);
	thread t1(&GridSimplifierM::simplifyT, this, map->lines[1], tri1, 1);
	thread t2(&GridSimplifierM::simplifyT, this, map->lines[2], tri1, 2);
	thread t3(&GridSimplifierM::simplifyT, this, map->lines[3], tri1, 3);

	t0.join();
	t1.join();
	t2.join();
	t3.join();

}

void GridSimplifierM::simplifyMTP(int limit){
	Polygon poly1(4, new Point*[4]);
	Polygon poly2(4, new Point*[4]);
	Polygon poly3(4, new Point*[4]);
	Polygon poly4(4, new Point*[4]);


	thread t0(&GridSimplifierM::simplifyTP, this, map->lines[0], poly1, 0);
	thread t1(&GridSimplifierM::simplifyTP, this, map->lines[1], poly2, 1);
	thread t2(&GridSimplifierM::simplifyTP, this, map->lines[2], poly3, 2);
	thread t3(&GridSimplifierM::simplifyTP, this, map->lines[3], poly4, 3);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();
}

void GridSimplifierM::wirteFile(string writeFile) {
	int length = map->linesNumber() * 155 + gridIndex->pointNumber() * 38;
	writeLinesM(map, writeFile, length);
}

//void GridSimplifierM::simplify(int limit){
//	Triangle triangle;
//	int last;
//	int removed = 1, total_removed = 0;
//	while (total_removed < limit && removed != 0){ //repeat the process if more points need to be removed
//		for (int i = 0; i < map->lines.size(); ++i){ //for each line
//			triangle.p[1] = map->lines[i]->points[0];
//			last = map->lines[i]->points.size();
//			if (map->lines[i]->points[last - 1]->leftInd == 0)
//				continue; // it is already a segment
//			triangle.p[2] = map->lines[i]->points[triangle.p[1]->rightInd];
//			if (*(triangle.p[1]) == map->lines[i]->points[last - 1]){ //if the line is a cycle
//				while (triangle.p[2]->rightInd < last){
//					if (map->lines[i]->kept == 4) //leave at least 4 points
//						break;
//					triangle.p[1] = triangle.p[2];
//					triangle.p[0] = map->lines[i]->points[triangle.p[1]->leftInd];
//					triangle.p[2] = map->lines[i]->points[triangle.p[1]->rightInd];
//					triangle.sort();
//					removeS(triangle);
//				}
//			}
//			else{
//				while (triangle.p[2]->rightInd < last){
//					triangle.p[1] = triangle.p[2];
//					triangle.p[0] = map->lines[i]->points[triangle.p[1]->leftInd];
//					triangle.p[2] = map->lines[i]->points[triangle.p[1]->rightInd];
//					triangle.sort();
//					removeS(triangle); //can I remove the current point?
//				}
//			}
//		}
//		removed = orig_size - gridIndex->size;
//		total_removed += removed;
//		orig_size = gridIndex->size;
//	}
//}



