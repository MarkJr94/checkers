/*
 * GameMaster_test.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: markjr
 */

#include "GameMaster.hpp"

int main()  {
	GameMaster gm (true);

//	gm.setDifficulty(3);

//	gm.testGame();

	gm.playAIvsAI();

//	gm.playAgainstAI();

	return 0;
}
