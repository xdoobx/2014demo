#include "TopoSimplifier.h"
using namespace std;


TopoSimplifier::TopoSimplifier(char* lineFile, char* pointFile){

	map = readLines(lineFile);
	points = readPoints(pointFile);
	gridIndex = new GridTree(map, points);
	orig_size = gridIndex->size;

	buildTopoGraph();
}


void TopoSimplifier::buildTopoGraph()
{
	for (int i = 0; i < map->lines.size(); i++)//go over all the lines
	{
		Line* line = map->lines[i];
		for (int j = 1; j < line->points.size()-1; j++)//go over all the line's points except the two endings.
		{
			
		}
	}
}

