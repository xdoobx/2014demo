#include "stdafx.h"
#include "GridTreeM.h"

GridTreeM::GridTreeM(LineSet* map, PointSet* points){
	range = Rect(map->minX - 0.1, map->maxX + 0.1, map->minY - 0.1, map->maxY + 0.1);
	divideW = 50;
	divideH = 50;
	gridW = (range.maxX - range.minX) / divideW;
	gridH = (range.maxY - range.minY) / divideH;
	
	size = 0;
	for (int i = 0; i < points->points.size(); ++i)
		insertM(points->points[i],0);

	for (int i = 0; i < map->lines.size(); ++i){
		for (int j = 0; j < map->lines[i]->points.size(); ++j){
			insertM(map->lines[i]->points[j], 0);
		}
	}
}

const Point* GridTreeM::insertM(Point* newPoint,int tid){
	int gridX = (newPoint->x - range.minX) / gridW;
	int gridY = (newPoint->y - range.minY) / gridH;
	//gridM[gridX][gridY][tid].push_back(newPoint); // even though this point overlays a previous point
	gridM[gridX][gridY][tid].push_back(newPoint); // even though this point overlays a previous point
	++size;
	return newPoint;
}

bool GridTreeM::hasPointInTri(const Triangle* triangle){
	int gridMinX = (triangle->minX - range.minX) / gridW;
	int gridMinY = (triangle->minY - range.minY) / gridH;
	int gridMaxX = (triangle->maxX - range.minX) / gridW;
	int gridMaxY = (triangle->maxY - range.minY) / gridH;
	for (int i = gridMinX; i <= gridMaxX; ++i){
		for (int j = gridMinY; j <= gridMaxY; ++j){

			//int vecN = threadN + 1;
			for (int l = 0; l < 4; l++)
			for (int k = 0; k < gridM[i][j][l].size(); ++k)
			{
				if (gridM[i][j][l][k]->kept && triangle->isInTri(gridM[i][j][l][k]->x, gridM[i][j][l][k]->y))
					return true;
			}

		}
	}
	return false;
}



//bool GridTreeM::hasPointInTri(const Triangle* triangle){
//	int gridMinX = (triangle->minX - range.minX) / gridW;
//	int gridMinY = (triangle->minY - range.minY) / gridH;
//	int gridMaxX = (triangle->maxX - range.minX) / gridW;
//	int gridMaxY = (triangle->maxY - range.minY) / gridH;
//	for (int i = gridMinX; i <= gridMaxX; ++i){
//		for (int j = gridMinY; j <= gridMaxY; ++j){
//
//			for (int k = 0; k < gridM[i][j].size(); ++k){
//			if (gridM[i][j][k]->kept && triangle->isInTri(gridM[i][j][k]->x, gridM[i][j][k]->y))
//			return true;
//			}
//		}
//	}
//	return false;
//}

