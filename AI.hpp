/*
 * AI.hpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */
#pragma once
#ifndef AI_HPP_
#define AI_HPP_

#include <vector>

#include "BitBoard.hpp"
#include "Save.hpp"
#include "Game.hpp"
//#include "DrawGame.hpp"

class AI {
public:
	friend class GameMaster;

	AI(const unsigned, const Save&, const Move& creator = {0,0}, const unsigned difficulty = 5);
	~AI();

	void printScene();

	Move getRandomMove() const;

	bool canMultiJump(const Game&);

	Move evaluateGame(Game&);

private:
	AI(const AI&);
	AI& operator=(const AI&);

	unsigned _level;
	unsigned _difficulty;
	std::vector<AI *> _children;
	Move _move;
	Save _save;
	float _p1Avg;
	float _p2Avg;

	static Game* _game;

	unsigned testMoves();

	unsigned generateOutcomes();

	Move evaluateMoves(bool optimizeForP1 = true, bool aggro = false);

	void updateScores();

	void resetToSave(const Save&);
};


#endif /* AI_HPP_ */
