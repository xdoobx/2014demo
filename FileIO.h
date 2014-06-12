#ifndef _FILEIO_H_
#define _FILEIO_H_

#include "GeoUtility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <time.h>

extern LineSet* readLines(string filename);
extern PointSet* readPoints(string filename);
extern void writeLines(LineSet* map, string filename, int length);
extern void readLinesT(LineSetM* map, char *pData, char* endMark, int threadID);
extern LineSetM* readLinesM(string filename);

#endif