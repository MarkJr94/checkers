/*
 * TranspositionalTable.h
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#pragma once
#ifndef TRANSPOSITIONTABLE_HPP_
#define TRANSPOSITIONTABLE_HPP_

#include <vector>

#include "Piece.hpp"
#include "Save.hpp"
#include "Game.hpp"
#include "DrawGame.hpp"
#include "Hash.hpp"

class AI;

struct TableEnt {
	MoveRecord move;
	bool done;

	TableEnt() :done(false) {}
};

typedef class TranspositionTable {
public:
	TranspositionTable();
	~TranspositionTable();

	bool hasEvaluated(const Game&);

	MoveRecord getMove( Game&);

	static Hash::Zkey hashGame(const Game&);

private:
	std::vector<TableEnt> entries;
	AI* ai;

	static unsigned tableSz;
} TT;

#endif /* TRANSPOSITIONTABLE_HPP_ */
