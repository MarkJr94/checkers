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
#include <memory>

#include "BitBoard.hpp"
#include "Save.hpp"
#include "Game.hpp"

class BaseAI {
public:
//	virtual void print_scene() = 0;

	virtual Move get_random_move() const = 0;

	virtual Move evaluate_game(Game&) = 0;

	virtual ~BaseAI() {};
};

class SimpleAI : public BaseAI {
public:
	friend class GameMaster;

	SimpleAI(const unsigned level = 0, const Save& = Save(), const unsigned difficulty = 6);
	~SimpleAI();

	void print_scene();

	Move get_random_move() const;

	Move evaluate_game(Game&);

	void difficulty(unsigned difficulty)
	{
		mDifficulty = difficulty;
	}

	unsigned difficulty() const
	{
		return mDifficulty;
	}

private:
	SimpleAI(const SimpleAI&);
	SimpleAI& operator=(const SimpleAI&);

	void generateMovesBlack();
	void generateMovesWhite();
	void generateMoves() {
		if (sGame.mTurn)
			generateMovesBlack();
		else
			generateMovesWhite();
	}


	void generateJumpsBlack();
	void generateJumpsWhite();
	void generateJumps() {
		if (sGame.mTurn)
			generateJumpsBlack();
		else
			generateJumpsWhite();
	}

	unsigned generateOutcomes();

	Move evaluateMoves(bool aggro = false);

	void updateScores(bool turn);

	void initialize(const Save&);

	unsigned mLevel;
	unsigned mDifficulty;
	std::vector<SimpleAI *> mChildren;
	std::vector<Move> mMoves;
	Save mSave;
	float mAvgScore;
//	float mP2Avg;

	static Game sGame;
};

#endif /* AI_HPP_ */
