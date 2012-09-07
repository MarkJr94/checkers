/*
 * AI_test.cpp
 *
 *  Created on: Sep 7, 2012
 *      Author: msinclair94
 */

#include <iostream>
#include "AI.hpp"

int main() {
	using std::cerr;
	using std::endl;
	Game g(false,false);
	AI a;

	g.print();

	cerr << _errtable[g.makeMove( { 9, 12 })] << std::endl;
	g.print();

	cerr << _errtable[g.makeMove( { 21, 17 })] << std::endl;
	g.print();

	auto moveinfo = a.evaluateGame(g);

	if (moveinfo.second) {
		cerr << _errtable[g.jump(moveinfo.first)] << endl;
	} else
		cerr << _errtable[g.makeMove(moveinfo.first)] << endl;;

//	for (auto & pair : bbUMap) {
//		cerr << pair.first << std::endl << pair.second <<std::endl;
//	}

	g.print();
	return 0;
}



