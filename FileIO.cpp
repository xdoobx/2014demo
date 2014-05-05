
#include "stdafx.h"
#include "FileIO.h"

LineSet* readLines(string filename)
{
	LineSet* map = new LineSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	string fileline;
	Point point;
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

	while (pos2 != -1){
		fileline = file.substr(pos1, pos2 - pos1);
		id = fileline.substr(0, fileline.find_first_of(':'));
		fileline = fileline.substr(120 + id.size());
		fileline = fileline.substr(0, fileline.size() - 35);
		Line line;
		line.id = id;
		strpos = (char*)fileline.c_str();
		while (strpos[0] != '\0'){
			point.x = strtod(strpos, &strpos);
			++strpos;
			point.y = strtod(strpos, &strpos);
			++strpos;
			if (map->maxX < point.x)
				map->maxX = point.x;
			else if (map->minX > point.x)
				map->minX = point.x;
			if (map->maxY < point.y)
				map->maxY = point.y;
			else if (map->minY > point.y)
				map->minY = point.y;
			point.pointInd = line.points.size();
			point.lineInd = map->lines.size();
			line.points.push_back(point);
			++line.kept;
		}
		line.points[0].kept = true;
		line.points[line.points.size() - 1].kept = true;
		map->lines.push_back(line);
		pos1 = pos2 + 1;
		pos2 = file.find('\n', pos1);
	}

	return map;
}

PointSet* readPoints(string filename)
{
	PointSet* points = new PointSet;
	ifstream fin(filename, std::ios::binary | std::ios::ate);
	string filepoint;
	Point point;
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

	while (pos2 != -1){
		filepoint = file.substr(pos1, pos2 - pos1);
		filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
		filepoint = filepoint.substr(filepoint.find_first_of('>') + 1);
		filepoint = filepoint.substr(0, filepoint.find_first_of('<'));

		point.x = strtod(filepoint.c_str(), &strpos);
		point.y = strtod(++strpos, NULL);

		if (points->maxX < point.x)
			points->maxX = point.x;
		else if (points->minX > point.x)
			points->minX = point.x;
		if (points->maxY < point.y)
			points->maxY = point.y;
		else if (points->minY > point.y)
			points->minY = point.y;

		points->points.push_back(point);
		pos1 = pos2 + 1;
		pos2 = file.find('\n', pos1);
	}

	return points;
}

void writeLines(LineSet* map, string filename)
{
	ofstream fout(filename);
	fout.precision(std::numeric_limits< double >::digits10);
	for (int i = 0; i < map->lines.size(); ++i){
		fout << map->lines[i].id << ':' << map->gmlLineString << map->gmlCoordinates;
		for (int j = 0; j < map->lines[i].points.size(); ++j){
			if (map->lines[i].points[j].kept)
				fout << map->lines[i].points[j].x << ',' << map->lines[i].points[j].y << ' ';
		}
		fout << map->endCoordinates << map->endLineString << endl;
	}
	fout.close();
}
