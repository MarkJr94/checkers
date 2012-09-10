/*
 * AI_test.cpp
 *
 *  Created on: Sep 7, 2012
 *      Author: msinclair94
 */

#include <iostream>
#include "AI.hpp"

void testmJump() {
	using std::cout;
	using std::endl;

	Game g(false, false);

	AI a;

	std::pair<Move, bool> moveinfo;

	do {
		cout << "P1: " << g.getP1score() <<"P2: " << g.getP2score() << endl;
		g.print();
		moveinfo = a.evaluateGame(g);

		if (moveinfo.second) {
			cout << _errtable[g.jump(moveinfo.first)] << endl;
		} else
			cout << _errtable[g.makeMove(moveinfo.first)] << endl;
	} while (g.getP1score() > 0 &&  g.getP2score() > 0);

}

int main() {
	testmJump();

	return 0;
}

