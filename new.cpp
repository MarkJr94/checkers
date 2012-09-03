/*
 * new.cpp
 *
 *  Created on: Aug 28, 2012
 *      Author: msinclair94
 */

#include <iostream>

#include "AI.hpp"
#include "DrawGame.hpp"

void testGame() {

	Game g(true, true);
	MoveRecord nextMove;
	AI* ai = new AI (1, g.getSave(), nextMove);

	bool success;
	unsigned numErrors = 0;

	do {
		g.print();
//		ai.resetToSave(g.getSave());
		ai = new AI (1, g.getSave(), nextMove);
		nextMove = ai->evaluateGame(g);
		std::cout << "Chosen Move: \n";
		printMove(nextMove);
		if (nextMove.jump) {
			success = g.jumpPiece(nextMove.piece, nextMove.prey);
		} else {
			success = g.movePiece(nextMove.piece, nextMove.dir);
		}

		delete ai;
		ai = NULL;

		if (!success) {
			std::cerr << "AN ERROR OCCURED\n";
			return;
		}
	} while (g.getP1score() * g.getP2score() > 0 && numErrors == 0);

	delete ai;

	std::cerr << "EXITING NORMALLY\n";
}

void testDrawGame() {
	DrawGame d(true, true);
	d.print();
	AI ai(1, d.getSave(), MoveRecord());
//	ai.printScene();
//	ai.generateOutcomes();
//	ai.recursivePrint();
	GameWin g;
	g.createGame();
	while (true) {
		if (g.sfHandleEvents())
			break;
		g.drawGame();
		g.Display();
	}
}

int main() {
	testGame();
	return 0;
}
