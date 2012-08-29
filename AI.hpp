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

class AI {
public:
	AI(unsigned degree, const Save& record, const MoveRecord& creator);
	~AI();

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
	std::vector<AI *> children;
	double p1Avg;
	double p2Avg;
	MoveRecord creator;
	Save save;
	bool done;

	static Game* scenario;
	static AI* hashTable[100000000];
	static unsigned tableSz;

	Hash::Zkey hashGame();
};

bool aiInteract(Game *theGame, const bool interact, MoveRecord& blank,
		AI* predictor, bool p1Turn);
/* Play Player vs Player
 * 	Arguments:
 * 		theGame: pointer to an allocated game
 */
void playPvP(Game *theGame);

/* Play Player vs AI
 * 	Arguments:
 * 		theGame: pointer to an allocated game
 * 		interact: whether to print interactive output.
 */
void playAgainstAI(Game *theGame, bool interact);

/* Play AI VS AI Game.
 * Arguments:
 * 	theGame: Pointer to an allocated game
 * 	interact: if interactive output should be printed
 */
void playAIvsAI(Game *theGame, bool interact);

#endif /* AI_HPP_ */
