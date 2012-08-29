/*
 * TranspositionalTable.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "TranspositionTable.hpp"
#include "AI.hpp"

/* Sleep portability thing */
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>

inline void delay( unsigned long ms )
{
	Sleep( ms );
}

#else  /* presume POSIX */

#include <unistd.h>

inline void delay(unsigned long ms) {
	usleep(ms * 1000);
}

#endif

unsigned TranspositionTable::tableSz = 100000000;

TT::TranspositionTable() :
		entries(tableSz), ai(NULL) {

}

TT::~TranspositionTable() {
	delete ai;
}

Hash::Zkey TranspositionTable::hashGame(const Game& game) {
	using namespace Hash;

	Zkey hash = 0;

	ZobristTable& zt = ZobristTable::instance();

	for (auto& p : game.p1) {
		hash ^=
				zt[p.second->isKing][Piece::BLACK][p.second->x + 8 * p.second->y];
	}

	for (auto& p : game.p2) {
		hash ^= zt[p.second->isKing][Piece::RED][p.second->x + 8 * p.second->y];
	}

	return hash;
}

bool TT::hasEvaluated(const Game& game) {
	using namespace Hash;

	Zkey z = hashGame(game);
	int bucket = z % tableSz;
	return entries[bucket].done;
}

MoveRecord TT::getMove(Game& game) {
	MoveRecord ret;
	using namespace Hash;

	Zkey z = hashGame(game);
	int bucket = z % tableSz;
	TableEnt& ent = entries[bucket];
	if (ent.done)
		return ent.move;

	ai = new AI(1, game.getSave(), MoveRecord());

	ret = ai->evaluateMoves();
	delete ai;
	ai = NULL;

	ent.done = true;
	ent.move = ret;
	return ret;
}
