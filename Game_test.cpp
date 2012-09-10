/*
 * Game_test.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: msinclair94
 */

#include "Game.hpp"
#include <iostream>

void tester1() {
	Game g(false, false);

	Save s = g.getSave();

	Save q;

	swap(s, q);

	g.print();
}

void tester2() {
	using std::cerr;
	Game g(false, false);

	Save s = g.getSave();

	Save q;

	swap(s, q);

	g.print();

	cerr << _errtable[g.makeMove( { 8, 12 })] << std::endl;
	g.print();

	cerr << _errtable[g.makeMove( { 21, 17 })] << std::endl;
	g.print();

	cerr << _errtable[g.jump( { 12, 17 })] << std::endl;
	g.print();

	cerr << _errtable[g.jump( { 26, 21 })] << std::endl;
	g.print();

//	cerr << _errtable[g.receiveInput()] << std::endl;
//	g.print();
//
//	cerr << _errtable[g.receiveInput()] << std::endl;
//	g.print();
}

int main() {

//	tester1();

	tester2();
	return 0;
}
