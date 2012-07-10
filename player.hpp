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
	/* Debug */
	bool debug;

public:
	/* Constructor */
	Player(const Piece::Color, bool db = true);

	/* Display player */
	void display() const;
	/* Get number of pieces */
	unsigned getnPieces() const { return numPieces;}
	/* Set number of pieces */
	void setnPieces(unsigned n) { numPieces = n;}
	/* get pointer to pieces */
	std::vector<Piece *> *getPieces() { return &pieces;}
	
	/* Destructor */
	~Player();
};

#endif /*PLAYER_HPP_ */
