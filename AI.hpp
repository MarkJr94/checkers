/*
 * AI.hpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */
#pragma once
#ifndef AI_HPP_
#define AI_HPP_

#include <utility>
#include <vector>
#include <sqlite3.h>

#include "BitBoard.hpp"
#include "Save.hpp"
#include "Game.hpp"

class AI {
public:
	friend class GameMaster;

	AI(const unsigned level = 0, const Save& = Save(), const unsigned difficulty = 5);
	~AI();

	void printScene();

	Move getRandomMove() const;

	std::pair<Move,bool> evaluateGame(Game&);

private:
	AI(const AI&);
	AI& operator=(const AI&);

	void generateMovesBlack();
	void generateMovesWhite();
	void generateMoves() {
		if (_game->_turn)
			generateMovesBlack();
		else
			generateMovesWhite();
	}


	void generateJumpsBlack();
	void generateJumpsWhite();
	void generateJumps() {
		if (_game->_turn)
			generateJumpsBlack();
		else
			generateJumpsWhite();
	}

	std::pair<bool,unsigned> generateOutcomes();

	std::pair<Move,bool> evaluateMoves(bool aggro = false);

	void updateScores();

	void initialize(const Save&);

	unsigned _level;
	unsigned _difficulty;
	std::vector<AI *> _children;
	std::vector<Move> _moves;
	Save _save;
	float _p1Avg;
	float _p2Avg;

	static Game* _game;
};

#endif /* AI_HPP_ */
