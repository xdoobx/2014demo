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
	bool operator==(const Point* point) const { return abs(x - point->x) < 0.001 && abs(y - point->y) < 0.001; }
	bool operator!=(const Point* point) const { return abs(x - point->x) >= 0.001 || abs(y - point->y) >= 0.001; }
	//bool operator==(const Point* point) const { return x == point->x && y == point->y; }
	//bool operator!=(const Point* point) const { return x != point->x || y != point->y; }
};

struct PointSet
{
	const char* gmlPoint = "<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">";
	const char* gmlCoordinates = "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
	const char* endCoordinates = "</gml:coordinates>";
	const char* endPoint = "</gml:Point>";
	vector<Point*> points;
	double minX;
	double maxX;
	double minY;
	double maxY;
	/*~PointSet(){
		for (unsigned int i = 0; i < points.size(); ++i)
			delete points[i];
	}*/
};

struct Line
{
	long id;
	int kept = 0;
	vector<Point*> points;
	vector<int>* shareEnd11 = new vector<int>();
	vector<int>* shareEnd12 = new vector<int>();
	vector<int>* shareEnd21 = new vector<int>();
	vector<int>* shareEnd22 = new vector<int>();
	vector<int>* shareEnd = new vector<int>();

	/*~Line(){
		delete shareEnd11;
		delete shareEnd12;
		delete shareEnd21;
		delete shareEnd22;
		delete shareEnd;
		for (int i = 0; i < points.size(); ++i)
			delete points[i];
	}*/
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

	void getShare(){
		for (int i = 0; i < lines.size(); ++i){
			for (int j = 0; j < lines[i]->shareEnd11->size(); ++j){
				for (int k = 0; k < lines[i]->shareEnd22->size(); ++k){
					if (lines[i]->shareEnd11->at(j) == lines[i]->shareEnd22->at(k))
						lines[i]->shareEnd->push_back(lines[i]->shareEnd11->at(j));
				}
			}
			for (int j = 0; j < lines[i]->shareEnd12->size(); ++j){
				for (int k = 0; k < lines[i]->shareEnd21->size(); ++k){
					if (lines[i]->shareEnd12->at(j) == lines[i]->shareEnd21->at(k))
						lines[i]->shareEnd->push_back(lines[i]->shareEnd12->at(j));
				}
			}
		}
	}
	/*~LineSet(){
		for (int i = 0; i < lines.size(); ++i)
			delete lines[i];
	}*/
};

struct Rect
{
	double maxX;
	double minX;
	double maxY;
	double minY;
	Rect(){}
	Rect(double x1, double x2, double y1, double y2) :
		minX(x1), maxX(x2), minY(y1), maxY(y2){}
	bool isInside(double x, double y){ 
		return x <= maxX && x > minX && y <= maxY && y > minY;
	}
};

//can be faster
struct Triangle
{
	Point* p[3];
	Triangle(){}
	Triangle(Point* P1, Point* P2, Point* P3){
		p[0] = P1;
		p[1] = P2; 
		p[2] = P3; 
		sortX();
		sortY();
	}
	double maxX;
	double minX;
	double maxY;
	double minY;
	inline void sortX(){ //I know you can understand this
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
	inline void sortY(){ //so does this
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
	inline bool isInTri(const double& x, const double& y) const {
		double prod1 = (x - p[1]->x)*(p[0]->y - y) - (x - p[0]->x)*(p[1]->y - y);
		double prod2 = (x - p[2]->x)*(p[0]->y - y) - (x - p[0]->x)*(p[2]->y - y);
		double prod3 = (x - p[2]->x)*(p[1]->y - y) - (x - p[1]->x)*(p[2]->y - y);

		return (prod1 > 0) != (prod2 > 0) && (prod2 > 0) != (prod3 > 0) && (prod1 != 0 || prod3 != 0);
	}
};

#endif