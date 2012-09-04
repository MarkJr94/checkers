/*
 * TranspositionalTable_test.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "TranspositionTable.hpp"
#include "Game.hpp"

int main() {
	using namespace std;
	using namespace Hash;
	TT table;

	Game g(false, false);
	cout << g.getHash() << " " << table.hasEvaluated(g.getHash()) << endl;
	g.print();
	table[g.getHash()]++;
	const Save& s = g.getSave();

	g.movePiece(9, RIGHT);
	cout << g.getHash() << " " << table.hasEvaluated(g.getHash()) << endl;
	g.print();

	g.movePiece(9, RIGHT);
	g.movePiece(12, RIGHT);
	g.jumpPiece(9, 9);
	cout << g.getHash() << " " << table.hasEvaluated(g.getHash()) << endl;
	g.print();

	g.restoreToSave(s);
	cout << g.getHash() << " " << table.hasEvaluated(g.getHash()) << endl;
	g.print();

	return 0;
}
