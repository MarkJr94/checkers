#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"

class GameTree {
	unsigned myScore;
	unsigned otherScore;
	Player p1;
	Player p2;
	bool turn;
	std::vector<std::vector<bool >> record;
	
public:
	GameTree(bool turn = true);
	void printRecord();
	void test(unsigned piece);
};

#endif /* GAME_TREE_ */
