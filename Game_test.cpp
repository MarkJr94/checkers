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

	g.makeMove( { { 2, 2 }, { 3, 3 } });
	g.print();

	g.makeMove( { { 1, 5 }, { 2, 4 } });
	g.print();

	Game g2(g.getSave(), true, true);

	g2.makeMove( { { 3, 3 }, { 1, 5 } });
	g2.print();
}

void tester2() {
	Game g(false, false);

	Save s = g.getSave();

	Save q;

	swap(s, q);

	g.print();

	g.makeMove( { { 2, 2 }, { 3, 3 } });
	g.print();

	g.makeMove( { { 1, 5 }, { 2, 4 } });
	g.print();
	g.getSave().write("lol.cks");

	q.read("lol.cks");

	Game g2(q, true, true);

	g2.makeMove( { { 3, 3 }, { 1, 5 } });
	g2.print();

	g2.makeMove( { { 0, 6 }, { 2, 4 } });
	g2.print();

	g2.receiveInput();
	g2.print();

	g2.receiveInput();
	g2.print();
}

int main() {
	Coord co { 4, 5 };
	Move m { co, co };

	std::cout << (int) m.src.x << "," << (int) m.src.y << " =>" << (int) m.dst.x
			<< "," << (int) m.dst.y << std::endl;

//	tester1();

	tester2();
	return 0;
}
