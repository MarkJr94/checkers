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

#include "BitBoard.hpp"
#include "Save.hpp"
#include "Game.hpp"

class AI {
public:
	friend class GameMaster;

	AI(const unsigned level = 0, const Save& = Save(), const unsigned difficulty = 6);
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
		if (sGame->mTurn)
			generateMovesBlack();
		else
			generateMovesWhite();
	}


	void generateJumpsBlack();
	void generateJumpsWhite();
	void generateJumps() {
		if (sGame->mTurn)
			generateJumpsBlack();
		else
			generateJumpsWhite();
	}

	std::pair<bool,unsigned> generateOutcomes();

	std::pair<Move,bool> evaluateMoves(bool aggro = false);

	void updateScores();

	void initialize(const Save&);

	unsigned mLevel;
	unsigned mDifficulty;
	std::vector<AI *> mChildren;
	std::vector<Move> mMoves;
	Save mSave;
	float mP1Avg;
	float mP2Avg;

	static Game* sGame;
};

#endif /* AI_HPP_ */
