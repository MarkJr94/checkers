/*
 * TranspositionalTable.h
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#pragma once
#ifndef TRANSPOSITIONALTABLE_H_
#define TRANSPOSITIONALTABLE_H_


#include <vector>

#include "Piece.hpp"
#include "Save.hpp"
#include "DrawGame.hpp"
#include "Hash.hpp"

struct MoveRecord {
	Game::Direction dir;
	bool jump;
	unsigned char piece;
	unsigned char prey;
};

class TranspositionalTable {
public:
	TranspositionalTable(unsigned degree, const Save& record, const MoveRecord& creator);
	~TranspositionalTable();

	void printScene();

	void recursivePrint();

	unsigned testMoves(const Save& savestate);

	void generateOutcomes(const int);

	void updateScores();

	bool canMultiJump(unsigned piece);

	MoveRecord getBestMove(bool optimizeForP2 = true, bool aggro = false);

	MoveRecord getCreator() const {
		return creator;
	}

	void clearToSave(const Save&);



private:
	unsigned degree;
	std::vector<TranspositionalTable *> children;
	double p1Avg;
	double p2Avg;
	MoveRecord creator;
	Save save;
	bool done;

	static Game* scenario;
	static TranspositionalTable* hashTable[100000000];
	static unsigned tableSz;

	Hash::Zkey hashGame();
};

#endif /* TRANSPOSITIONALTABLE_H_ */
