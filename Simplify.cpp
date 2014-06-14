// Simplify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GridSimplifier.h"
#include "GridSimplifierM.h"
#include <iostream>
using namespace std;

int _tmain(int argc, char* argv[])
{
	//clock_t total = clock(), begin = clock();
	clock_t end, begin = clock();

	//modify DEBUG->Properties->Configration Property->Character set, to "Use Multi-Byte Character Set"
	{
		//NaiveSimplifier simp((char*)argv[2], (char*)argv[3]);
		//GridSimplifier simp((char*)argv[2], (char*)argv[3]);
		GridSimplifierM simp((char*)argv[2], (char*)argv[3]);

		simp.simplifyMT(atoi(argv[1]));
		simp.wirteFile((char*)argv[4]);
	}
	end = clock();
	//cout << "write file: " << end - begin << endl;
	cout << "all\t" << end - begin << endl;
	return 0;
}
