// project.cpp : Defines the entry point for the console application.
//
/*
Name: Michael Davenport
Date due: 02/04/20
Dr. Jhijiang Dong CSCI4160
*/
#include "slp.h"

#include "prog.h"

using namespace std;

int main(void)
{
	SymbolTable		symbols;

	cout << "Program prog1 produce the following output:" << endl;
	prog1.interp(symbols);
	cout << endl;

	symbols.clear();
	cout << "Program prog2 produce the following output:" << endl;
	prog2.interp(symbols);
	cout << endl;

	symbols.clear();
	cout << "Program prog3 produce the following output:" << endl;
	prog3.interp(symbols);
	cout << endl;
	system("pause");
	return 0;
}

