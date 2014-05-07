
#include "stdafx.h"
#include "FileIO.h"

LineSet* readLines(string filename)
{
	LineSet* map = new LineSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	char* strpos;
	char* firstp;

	vector<char> vfile(fin.tellg());
	fin.seekg(0, ios::beg);
	fin.read(vfile.data(), vfile.size());
	vfile.push_back('\0');
	strpos = vfile.data();

	strtol(strpos, &firstp, 10);
	firstp += 120;
	map->maxX = map->minX = strtod(firstp, &firstp);
	map->maxY = map->minY = strtod(++firstp, NULL);

	//clock_t begin = clock();
	while (strpos[0] != '\0'){
		Line* line = new Line;
		line->id = strtol(strpos, &strpos, 10);
		strpos += 120;
		while (strpos[0] != '<'){
			Point* point = new Point;
			point->x = strtod(strpos, &strpos);
			++strpos;
			point->y = strtod(strpos, &strpos);
			++strpos;
			if (map->maxX < point->x)
				map->maxX = point->x;
			else if (map->minX > point->x)
				map->minX = point->x;
			if (map->maxY < point->y)
				map->maxY = point->y;
			else if (map->minY > point->y)
				map->minY = point->y;
			point->pointInd = line->points.size();
			point->lineInd = map->lines.size();
			line->points.push_back(point);
			++line->kept;
		}
		strpos += 36;
		map->lines.push_back(line);
	}
	//cout << "construct line cost: " << clock() - begin << endl;
	return map;
}

PointSet* readPoints(string filename)
{
	PointSet* points = new PointSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	string filepoint;
	char* strpos;
	char* firstp;

	vector<char> vfile(fin.tellg());
	fin.seekg(0, ios::beg);
	fin.read(vfile.data(), vfile.size());
	vfile.push_back('\0');
	strpos = vfile.data();

	strtol(strpos, &firstp, 10);
	firstp += 115;
	points->maxX = points->minX = strtod(strpos, &firstp);
	points->maxY = points->minY = strtod(++firstp, NULL);

	while (strpos[0] != '\0'){
		Point* point = new Point;
		strtol(strpos, &strpos, 10);
		strpos += 115;

		point->x = strtod(strpos, &strpos);
		++strpos;
		point->y = strtod(strpos, &strpos);
		++strpos;

		if (points->maxX < point->x)
			points->maxX = point->x;
		else if (points->minX > point->x)
			points->minX = point->x;
		if (points->maxY < point->y)
			points->maxY = point->y;
		else if (points->minY > point->y)
			points->minY = point->y;

		strpos += 31;
		points->points.push_back(point);
	}
	return points;
}



void writeLines(LineSet* map, string filename)
{
	FILE * pFile;
	fopen_s(&pFile, filename.c_str(), "wb");
	stringstream output;
	output.precision(std::numeric_limits< double >::digits10);
	for (int i = 0; i < map->lines.size(); ++i){
		output << map->lines[i]->id << map->gmlLineString << map->gmlCoordinates;
		for (int j = 0; j < map->lines[i]->points.size(); ++j){
			if (map->lines[i]->points[j]->kept)
				output << map->lines[i]->points[j]->x << ',' << map->lines[i]->points[j]->y << ' ';
		}
		output << map->endCoordinates << map->endLineString << endl;
	}
	string result(output.str());
	fwrite(result.c_str(), sizeof(char), result.size(), pFile);
	fclose(pFile);
}
