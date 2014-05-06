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
	bool operator==(const Point* point){ return x == point->x && y == point->y; }
	bool operator==(const Point point){ return x == point.x && y == point.y; }
	bool operator!=(const Point* point){ return x != point->x || y != point->y; }
	bool operator!=(const Point point){ return x != point.x || y != point.y; }
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
	vector<int>* shareEnd11 = new vector<int>();
	vector<int>* shareEnd12 = new vector<int>();
	vector<int>* shareEnd21 = new vector<int>();
	vector<int>* shareEnd22 = new vector<int>();

	~Line(){
		delete shareEnd11;
		delete shareEnd12;
		delete shareEnd21;
		delete shareEnd22;
	}
};

struct LineSet
{
	const char* gmlLineString = "<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">";
	const char* gmlCoordinates = "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
	const char* endCoordinates = "</gml:coordinates>";
	const char* endLineString = "</gml:LineString>";
	vector<Line*> lines;
	double minX;
	double maxX;
	double minY;
	double maxY;
	~LineSet(){ for (int i = 0; i < lines.size(); ++i) delete lines[i]; }
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

//can be faster
struct Triangle
{
	Point* p[3];
	Triangle(){}
	Triangle(Point* P1, Point* P2, Point* P3){ p[0] = P1; p[1] = P2; p[2] = P3; sortX(); sortY(); }
	double maxX;
	double minX;
	double maxY;
	double minY;
	inline void sortX(){
		if (p[0]->x < p[1]->x){
			if (p[0]->x < p[2]->x){
				minX = p[0]->x;
				if (p[1]->x > p[2]->x)
					maxX = p[1]->x;
				else
					maxX = p[2]->x;
			}
			else{
				minX = p[2]->x;
				maxX = p[1]->x;
			}
		}
		else{
			if (p[1]->x < p[2]->x){
				minX = p[1]->x;
				if (p[0]->x > p[2]->x)
					maxX = p[0]->x;
				else
					maxX = p[2]->x;
			}
			else{
				minX = p[2]->x;
				maxX = p[0]->x;
			}
		}
	}
	inline void sortY(){
		if (p[0]->y < p[1]->y){
			if (p[0]->y < p[2]->y){
				minY = p[0]->y;
				if (p[1]->y > p[2]->y)
					maxY = p[1]->y;
				else
					maxY = p[2]->y;
			}
			else{
				minY = p[2]->y;
				maxY = p[1]->y;
			}
		}
		else{
			if (p[1]->y < p[2]->y){
				minY = p[1]->y;
				if (p[0]->y > p[2]->y)
					maxY = p[0]->y;
				else
					maxY = p[2]->y;
			}
			else{
				minY = p[2]->y;
				maxY = p[0]->y;
			}
		}
	}
};

#endif