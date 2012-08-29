/*
 * TranspositionalTable.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */

#include "TranspositionalTable.h"

#include <iostream>

#include "TranspositionalTable.hpp"

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

Game* TranspositionalTable::scenario;

TranspositionalTable* TranspositionalTable::hashTable[100000000];

unsigned TranspositionalTable::tableSz = 100000000;

TranspositionalTable::TranspositionalTable(unsigned degree, const Save& record, const MoveRecord& creator) :
		degree(degree), children(), save(record), done(false) {
	if (degree > 1) {
		this->creator.dir = creator.dir;
		this->creator.jump = creator.jump;
		this->creator.piece = creator.piece;
		this->creator.prey = creator.prey;
	}

	scenario = new Game(record, false, false);

	scenario->restoreToSave(record);
	p1Avg = scenario->getP1score();
	p2Avg = scenario->getP2score();
	p2Avg = scenario->p2.size();


	int bucket = hashGame() % tableSz;
	if (hashTable[bucket]) {
		done = true;
	} else {
		hashTable[bucket] = this;
	}
}

TranspositionalTable::~TranspositionalTable() {
	for (auto child : children)
		delete child;
}

void TranspositionalTable::printScene() {
	using namespace std;

	cout << "This is a degree " << degree << " TranspositionalTable with " << "P1 Min: " << p1Avg
			<< "\tP2 Min: " << p2Avg << endl;
	Save oldsave = scenario->getSave();
	scenario->restoreToSave(save);
	scenario->print();
	scenario->restoreToSave(oldsave);
}

void TranspositionalTable::recursivePrint() {
	printScene();
	if (children.empty())
		return;

	for (auto& child : children) {
		child->recursivePrint();
	}
}

unsigned TranspositionalTable::testMoves(const Save& savestate) {
	using namespace std;

	bool retval;
	unsigned successCount = 0;

	MoveRecord origin;
	scenario->restoreToSave(savestate);

	unsigned jumpCount = 0;
	for (unsigned piece = 1; piece <= 12; piece++) {
		/* Test jumping */
		for (unsigned prey = 1; prey <= 12; prey++) {
			retval = scenario->jumpPiece(piece, prey);
			if (retval) {
				++jumpCount;
				++successCount;
				origin.jump = true;
				origin.piece = piece;
				origin.prey = prey;
//				if (canMultiJump(piece)) {
//					scenario.setTurn(!scenario.getTurn());
//				} else {
//					scenario.setMustJump(0);
//				}
				children.push_back(
						new TranspositionalTable(degree + 1, scenario->getSave(), origin));
				scenario->restoreToSave(savestate);
			}
		}
	}
	if (jumpCount > 0)
		return successCount;

	for (unsigned piece = 1; piece <= 12; piece++) {

		/* Test left move */
		retval = scenario->movePiece(piece, DrawGame::LEFT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = DrawGame::LEFT;
			children.push_back(new TranspositionalTable(degree + 1, scenario->getSave(), origin));
			scenario->restoreToSave(savestate);
		}

		/* Test right move */
		retval = scenario->movePiece(piece, DrawGame::RIGHT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = DrawGame::RIGHT;
			children.push_back(new TranspositionalTable(degree + 1, scenario->getSave(), origin));
			scenario->restoreToSave(savestate);
		}

		/* Test back right move */
		retval = scenario->movePiece(piece, DrawGame::BKRIGHT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = DrawGame::BKRIGHT;
			children.push_back(new TranspositionalTable(degree + 1, scenario->getSave(), origin));
			scenario->restoreToSave(savestate);
		}

		/* Test back left move */
		retval = scenario->movePiece(piece, DrawGame::BKLEFT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = DrawGame::BKLEFT;
			children.push_back(new TranspositionalTable(degree + 1, scenario->getSave(), origin));
			scenario->restoreToSave(savestate);
		}

	}
	return successCount;
}

void TranspositionalTable::generateOutcomes(const int nLevels) {

	int next = nLevels - 1;
	if (next <= 1)
		return;

	if (done) return;

//	Save savestate = scenario.getSave();
	Save savestate = save;
	unsigned num = testMoves(savestate);
	if (num < 1)
		return;

	size_t numKids = children.size();
	for (size_t i = 0; i < numKids; i++) {
		children[i]->generateOutcomes(next);
	}
}

void TranspositionalTable::updateScores() {
	if (children.empty())
		return;

	for (auto& child : children)
		child->updateScores();

	double temp1 = 0, temp2 = 0;
	for (auto& child : children) {
		temp1 += child->p1Avg;
		temp2 += child->p2Avg;
	}

	p1Avg = temp1 / children.size();
	p2Avg = temp2 / children.size();
}

MoveRecord TranspositionalTable::getBestMove(bool optimizeForP2, bool aggro) {
	size_t nKids = children.size();

	if (nKids == 0) {
		std::cerr << "No Moves.\n";
		creator.piece = 0xff;
		return creator;
	}

	size_t favoredSon = 0;
	double bestAvg = 0.0;
	if (!aggro) {
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p2Avg > bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p1Avg > bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		}
	} else {
		bestAvg = 999999999;
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p1Avg < bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p2Avg < bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		}
	}
	return children[favoredSon]->getCreator();
}

bool TranspositionalTable::canMultiJump(unsigned piece) {
	bool retval = false;
	scenario->setTurn(!scenario->getTurn());
	Save savestate = scenario->getSave();
	/* Test jumping */
	for (unsigned prey = 1; prey <= 12; prey++) {
		retval = scenario->jumpPiece(piece, prey);
		if (retval) {
			scenario->restoreToSave(savestate);
			return true;
		}
	}

	scenario->restoreToSave(savestate);
	return retval;
}

void TranspositionalTable::clearToSave(const Save& saveState) {
	children.clear();
	scenario->restoreToSave(saveState);
	p1Avg = scenario->getP1score();
	p2Avg = scenario->getP2score();
}

Hash::Zkey TranspositionalTable::hashGame() {
	using namespace Hash;
	scenario->restoreToSave(save);

	Zkey hash = 0;

	ZobristTable& zt = ZobristTable::instance();

	for (auto& p : scenario->p1) {
		hash ^= zt[p.second->isKing][Piece::BLACK][p.second->x + 8 * p.second->y];
	}

	for (auto& p : scenario->p2) {
		hash ^= zt[p.second->isKing][Piece::RED][p.second->x + 8 * p.second->y];
	}

	int bucket = hash % tableSz;
	if (hashTable[bucket]) {
		std::cerr << "already done";
	}
//	hashTable[bucket]++;

	return hash;
}

