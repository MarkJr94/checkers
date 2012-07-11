#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"

class GameTree {
	Match scenario;
	std::vector<GameTree> children;
	
public:
	GameTree(const SaveGame&);
	void printScene();
	void test();
	void generateChildren();
};

#endif /* GAME_TREE_ */
