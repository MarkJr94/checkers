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

#include "checkers.hpp"
#include "Save.hpp"
#include "DrawGame.hpp"

struct MoveRecord {
	DrawGame::Direction dir;
	bool jump;
	unsigned char piece;
	unsigned char prey;
};

class AI {
public:
	AI(unsigned degree, const Save& record, const MoveRecord& creator);
	~AI();

	void printScene();

	void recursivePrint();

	unsigned testMoves(const Save& savestate);

	void generateOutcomes();

	void updateScores();

	bool canMultiJump(unsigned piece);

	MoveRecord getBestMove(bool optimizeForP2 = true, bool aggro = false);

	MoveRecord getCreator() const {
		return creator;
	}

	void clearToSave(const Save&);

private:
	unsigned degree;
	static DrawGame scenario;
	std::vector<AI *> children;
	double p1Avg;
	double p2Avg;
	MoveRecord creator;
	Save save;
};

#endif /* AI_HPP_ */
