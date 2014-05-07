
#include "stdafx.h"
#include "FileIO.h"

LineSet* readLines(string filename)
{
	LineSet* map = new LineSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	string fileline;
	char* strpos;
	string id;

	vector<char> vfile(fin.tellg());
	fin.seekg(0, ios::beg);
	fin.read(vfile.data(), vfile.size());
	string file(vfile.data());
	int pos1 = 0, pos2 = file.find('\n');

	fileline = file.substr(pos1, pos2 - pos1);
	id = fileline.substr(0, fileline.find_first_of(':'));
	fileline = fileline.substr(120 + id.size());
	fileline = fileline.substr(0, fileline.size() - 35);
	map->maxX = map->minX = strtod(fileline.c_str(), &strpos);
	map->maxY = map->minY = strtod(++strpos, &strpos);

	clock_t begin = clock();
	while (pos2 != -1){
		fileline = file.substr(pos1, pos2 - pos1);
		id = fileline.substr(0, fileline.find_first_of(':'));
		fileline = fileline.substr(120 + id.size());
		fileline = fileline.substr(0, fileline.size() - 35);
		Line* line = new Line;
		line->id = id;
		strpos = (char*)fileline.c_str();
		while (strpos[0] != '\0'){
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
		map->lines.push_back(line);
		pos1 = pos2 + 1;
		pos2 = file.find('\n', pos1);
	}
	cout << "construct line cost: " << clock() - begin << endl;
	return map;
}

PointSet* readPoints(string filename)
{
	PointSet* points = new PointSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	string filepoint;
	char* strpos;

	vector<char> vfile(fin.tellg());
	fin.seekg(0, ios::beg);
	fin.read(vfile.data(), vfile.size());
	string file(vfile.data());
	int pos1 = 0, pos2 = file.find('\n');

	filepoint = file.substr(pos1, pos2 - pos1);
	filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
	filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
	filepoint = filepoint.substr(0, filepoint.find_first_of('<'));
	points->maxX = points->minX = strtod(filepoint.c_str(), &strpos);
	points->maxY = points->minY = strtod(++strpos, &strpos);

	clock_t begin = clock();
	while (pos2 != -1){
		Point* point = new Point;
		filepoint = file.substr(pos1, pos2 - pos1);
		filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
		filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
		filepoint = filepoint.substr(0, filepoint.find_first_of('<'));

		point->x = strtod(filepoint.c_str(), &strpos);
		point->y = strtod(++strpos, NULL);

		if (points->maxX < point->x)
			points->maxX = point->x;
		else if (points->minX > point->x)
			points->minX = point->x;
		if (points->maxY < point->y)
			points->maxY = point->y;
		else if (points->minY > point->y)
			points->minY = point->y;

		points->points.push_back(point);
		pos1 = pos2 + 1;
		pos2 = file.find('\n', pos1);
	}
	return points;
}

void writeLines(LineSet* map, string filename)
{
	FILE * pFile;
	fopen_s(&pFile, filename.c_str(), "w");
	stringstream output;
	output.precision(std::numeric_limits< double >::digits10);
	for (int i = 0; i < map->lines.size(); ++i){
		output << map->lines[i]->id << ':' << map->gmlLineString << map->gmlCoordinates;
		for (int j = 0; j < map->lines[i]->points.size(); ++j){
			if (map->lines[i]->points[j]->kept)
				output << map->lines[i]->points[j]->x << ',' << map->lines[i]->points[j]->y << ' ';
		}
		output << map->endCoordinates << map->endLineString << endl;
	}
	string result = output.str();
	fwrite(result.c_str(), sizeof(char), result.size(), pFile);
	fclose(pFile);
}
