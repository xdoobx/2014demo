#ifndef _GEOUTILITY_H_
#define _GEOUTILITY_H_

#include <vector>
using namespace std;

struct Point
{
	double x;
	double y;
	bool kept = true;
	int lineInd = -1;
	int pointInd = -1;
	Point(){}
	Point(double X, double Y) :x(X), y(Y){}
	bool operator==(Point* point){ return x == point->x && y == point->y; }
	bool operator==(Point point){ return x == point.x && y == point.y; }
};

struct PointSet
{
	const char* gmlPoint = "<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">";
	const char* gmlCoordinates = "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
	const char* endCoordinates = "</gml:coordinates>";
	const char* endPoint = "</gml:Point>";
	vector<Point> points;
	double minX;
	double maxX;
	double minY;
	double maxY;
};

struct Line
{
	string id;
	int kept = 0;
	vector<Point> points;
	vector<int>* share = NULL;
	~Line(){ delete share; }
};

struct LineSet
{
	const char* gmlLineString = "<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">";
	const char* gmlCoordinates = "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
	const char* endCoordinates = "</gml:coordinates>";
	const char* endLineString = "</gml:LineString>";
	vector<Line> lines;
	double minX;
	double maxX;
	double minY;
	double maxY;
};

struct Rect
{
	double maxX;
	double minX;
	double maxY;
	double minY;
	Rect(){}
	Rect(double x1, double x2, double y1, double y2) :minX(x1), maxX(x2),
		minY(y1), maxY(y2){}
	bool isInside(double x, double y){ return x <= maxX && x > minX && y <= maxY && y > minY; }
};

struct Triangle
{
	Point* p[3];
	Triangle(){}
	Triangle(Point* P1, Point* P2, Point* P3){ p[0] = P1; p[1] = P2; p[2] = P3; }
	double minX(){
		if (p[0]->x < p[1]->x && p[0]->x < p[2]->x)
			return p[0]->x;
		else if (p[1]->x < p[2]->x)
			return p[1]->x;
		else
			return p[2]->x;
	}
	double maxX(){
		if (p[0]->x > p[1]->x && p[0]->x > p[2]->x)
			return p[0]->x;
		else if (p[1]->x > p[2]->x)
			return p[1]->x;
		else
			return p[2]->x;
	}
	double minY(){
		if (p[0]->y < p[1]->y && p[0]->y < p[2]->y)
			return p[0]->y;
		else if (p[1]->y < p[2]->y)
			return p[1]->y;
		else
			return p[2]->y;
	}
	double maxY(){
		if (p[0]->y > p[1]->y && p[0]->y > p[2]->y)
			return p[0]->y;
		else if (p[1]->y > p[2]->y)
			return p[1]->y;
		else
			return p[2]->y;
	}
};

#endif