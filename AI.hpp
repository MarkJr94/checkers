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
#include "Game.hpp"
#include "DrawGame.hpp"

class AI {
public:
	friend class GameMaster;

	AI(const unsigned, const Save&, const MoveRecord& creator = MoveRecord(), const unsigned difficulty = 5);
	~AI();

	void printScene();

	MoveRecord getRandomMove() const;

	bool canMultiJump(const Game&);

	MoveRecord evaluateGame(Game&);

private:
	AI(const AI&);
	AI& operator=(const AI&);

	unsigned _level;
	unsigned _difficulty;
	std::vector<AI *> _children;
	MoveRecord _move;
	Save _save;
	float _p1Avg;
	float _p2Avg;

	static Game* _game;

	unsigned testMoves();

	unsigned generateOutcomes();

	MoveRecord evaluateMoves(bool optimizeForP1 = true, bool aggro = false);

	void updateScores();

	void resetToSave(const Save&);
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

void printMove(const MoveRecord&);

#endif /* AI_HPP_ */
