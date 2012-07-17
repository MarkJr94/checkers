#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"

class GameTree {
public:
	unsigned level;
	Match scenario;
	std::vector<GameTree *> children;
	double p1Min;
	double p2Min;
	
public:
	GameTree(unsigned level, const SaveGame);
	void printScene();
	unsigned testMoves(SaveGame savestate);
	void recurse();
	std::vector<GameTree *> kidnap();
	void updateScores();
	void recursivePrint();
};

#endif /* GAME_TREE_ */
