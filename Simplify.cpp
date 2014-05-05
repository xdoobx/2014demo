// Simplify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simplifier.h"

int _tmain(int argc, char* argv[])
{
	clock_t begin = clock();
	clock_t end;

	Simplifier simp((char*)argv[1], (char*)argv[2]);
	simp.simplify();
	simp.wirteFile((char*)argv[3]);
	end = clock();
	cout << "all\t" << end - begin << endl;
	return 0;
}

