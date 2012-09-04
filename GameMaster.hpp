/*
 * GameMaster.hpp
 *
 *  Created on: Sep 3, 2012
 *      Author: markjr
 */

#ifndef GAMEMASTER_HPP_
#define GAMEMASTER_HPP_

#include "Piece.hpp"
#include "Game.hpp"
#include "Save.hpp"
#include "Hash.hpp"
#include "TranspositionTable.hpp"
#include "AI.hpp"


class GameMaster {
public:
	GameMaster(const bool interact = false);
	~GameMaster();

	void playAIvsAI();

	void playPvP();

	void playAgainstAI();

	void testGame();

	void setDifficulty(const unsigned);

private:
	Game* _game;
	AI * _ai;
	MoveRecord _move;
	bool _interact;
	TT _table;

	bool aiInteract();
};

inline void delay(const unsigned long);

#endif /* GAMEMASTER_HPP_ */
