// Simplify.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simplifier.h"

int _tmain(int argc, char* argv[])
{
	clock_t begin = clock();
	clock_t end;

	//modify DEBUG->Properties->Configration Property->Character set, to "Use Multi-Byte Character Set"
	Simplifier simp((char*)argv[2], (char*)argv[3]);
	simp.simplify(atoi(argv[1]));
	simp.wirteFile((char*)argv[4]);
	end = clock();
	cout << "all\t" << end - begin << endl;
	return 0;
}

