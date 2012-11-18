/*
 * Game_test.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: msinclair94
 */

#include "Game.hpp"
#include <iostream>

void tester1()
{
	Game g(false, false);

	Save s = g.getSave();

	Save q;

	swap(s, q);

	g.print();
}

void tester2()
{
	using std::cerr;
	Game g(false, false);

	Save s = g.getSave();

	Save q;

	swap(s, q);

	g.print();

	cerr << errorTable[g.move( { 8, 12, false })] << std::endl;
	g.print();

	cerr << errorTable[g.move( { 21, 17, false })] << std::endl;
	g.print();

	cerr << errorTable[g.move( { 12, 17, true })] << std::endl;
	g.print();

	cerr << errorTable[g.move( { 26, 21, true })] << std::endl;
	g.print();

//	cerr << _errtable[g.receiveInput()] << std::endl;
//	g.print();
//
//	cerr << _errtable[g.receiveInput()] << std::endl;
//	g.print();
}

int main()
{

//	tester1();

	tester2();
	return 0;
}
