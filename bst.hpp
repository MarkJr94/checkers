#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"

class GameTree {
	unsigned level;
	Match scenario;
	std::vector<GameTree> children;
	
public:
	GameTree(unsigned level, const SaveGame);
	void printScene();
	void generateChildren();
	unsigned testMoves(SaveGame savestate);
	bool testJumps(unsigned,SaveGame);
	void recurse();
	std::vector<GameTree> kidnap() { return children; }
};

#endif /* GAME_TREE_ */
