/*
 * GameWin_test.cpp
 *
 *  Created on: Sep 9, 2012
 *      Author: markjr
 */

#include "GameWin.hpp"

int main() {

	GameWin G(800,800);

	G.bindGame(new Game(false,false));

	G.loop();
}

