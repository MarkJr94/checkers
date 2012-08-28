/*
 * new.cpp
 *
 *  Created on: Aug 28, 2012
 *      Author: msinclair94
 */

#include "AI.hpp"
#include "DrawGame.hpp"

int main() {
	DrawGame d(true, true);
	d.print();
	AI ai(1, d.getSave(), MoveRecord());
//	ai.printScene();
//	ai.generateOutcomes();
//	ai.recursivePrint();
	GameWin g ;
	g.createGame();
	while (true) {
		if (g.sfHandleEvents()) break;
		g.drawGame();
		g.Display();
	}
	return 0;
}
