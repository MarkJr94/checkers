#ifndef GAME_TREE_
#define GAME_TREE_

#include <vector>
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"

struct MoveRecord {
	Game::Direction dir;
	bool jump;
	unsigned char piece;
	unsigned char prey;
};

class GameTree {
private:
	unsigned level;
	Game scenario;
	std::vector<GameTree *> children;
	double p1Avg;
	double p2Avg;
	MoveRecord creator;

public:
	GameTree(unsigned level, const SaveGame, const MoveRecord creator);
	~GameTree();
	void printScene();
	unsigned testMoves(SaveGame savestate);
	void generateOutcomes();
	std::vector<GameTree *> kidnap();
	void updateScores();
	void recursivePrint();

	MoveRecord getBestMove(bool optimizeForP2 = true, bool aggro = false);
	bool canMultiJump(unsigned piece);

	inline MoveRecord getCreator() {
		return creator;
	}

	unsigned getLevel() const {
		return level;
	}

	double getP1Avg() const {
		return p1Avg;
	}

	void setP1Avg(double p1Avg) {
		this->p1Avg = p1Avg;
	}

	double getP2Avg() const {
		return p2Avg;
	}

	void setP2Avg(double p2Avg) {
		this->p2Avg = p2Avg;
	}

	Game getScenario() const {
		return scenario;
	}
};

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

#endif /* GAME_TREE_ */
