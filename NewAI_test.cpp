/*
 * NewAI_test.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: mark
 */

#include "NewAI.hpp"

#include <iostream>

void testmJump() {
	using std::cout;
	using std::endl;

	Game g(false, false);

	NewAI a(15);

	Move move;

	do {
		cout << "P1: " << g.getP1score() <<"P2: " << g.getP2score() << endl;
		g.print();
		move = a.evaluate_game(g);

		cout << errorTable[g.move(move)];
	} while (g.getP1score() > 0 &&  g.getP2score() > 0);

	g.print();

}

int main() {
	testmJump();

	return 0;
}
