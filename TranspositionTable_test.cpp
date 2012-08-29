/*
 * TranspositionalTable_test.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "TranspositionTable.hpp"

int main() {
	using namespace std;
	TT table;

	Game g(false, false);
	int i = 20;
	while (i-- >= 0) {
		cout << TT::hashGame(g) << endl;
		cout << table.hasEvaluated(g) << std::endl;
	}

	Save s = g.getSave();

	g.print();

	MoveRecord m = table.getMove(g);

	if (m.jump) {
		g.jumpPiece(m.piece, m.prey);
	} else {
		g.movePiece(m.piece, m.dir);
	}

	g.print();

	return 0;
}
