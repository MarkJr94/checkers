/*
 * GameWin_test.cpp
 *
 *  Created on: Sep 9, 2012
 *      Author: markjr
 */

#include "SFMLGame.hpp"

int main() {

	SFMLGame G(900,1000);

	G.bindGame(new Game(false,false));

	G.loop();
}

