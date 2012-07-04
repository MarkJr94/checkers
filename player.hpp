#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "checkers.hpp"

class Player {
	/* Array stores pointers to pieces on board inplay */
	std::vector<Piece *> pieces;
	/* Player color */
	Piece::Color col;
	/* Number of pieces in play */
	unsigned numPieces;
	/* Static game board */
	static std::vector<std::vector<Piece> > game;
	/* Debug */
	bool debug;

public:
	/* Enumerations for movement and jump directions */
	enum Direction {LEFT, RIGHT};
	enum KingDi {UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};
	/* Constructor */
	Player(const Piece::Color, bool db = true);
	/* Get number of pieces */
	unsigned getnPieces() const;
	/* Display player */
	void display() const;
	/* Piece movement */
	bool movePiece(unsigned piece, Direction d);
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey, Player& other);
	/* Print the game */
	void printgame() const;
};

#endif /*PLAYER_HPP_ */
