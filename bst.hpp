#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"

struct MoveRecord {
	Match::Direction dir;
	bool jump;
	unsigned char piece;
	unsigned char prey;
};

class GameTree {
private:
	unsigned level;
	Match scenario;
	std::vector<GameTree *> children;
	double p1Avg;
	double p2Avg;
	MoveRecord creator;
	
public:
	GameTree(unsigned level, const SaveGame, const MoveRecord creator);
	void printScene();
	unsigned testMoves(SaveGame savestate);
	void recurse();
	std::vector<GameTree *> kidnap();
	void updateScores();
	void recursivePrint();
	inline MoveRecord getCreator() { return creator;}
	MoveRecord getBestMove(bool optimizeForP2 = true);
};

#endif /* GAME_TREE_ */
