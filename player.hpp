#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "checkers.hpp"

class Player {

private:
	std::vector<Piece> pieces;
	Color col;
	
public:
	static std::vector<std::vector<Piece> > game;
	unsigned numPieces;
	
	Player(const Color);
	void display() const;
	void movePiece(unsigned piece, Direction d)
	{
		if (col == BLACK) 
			movePieceBlack(piece,d);
		else 
			movePieceRed(piece,d);
	}
	void movePieceRed(unsigned piece, Direction d);
	void movePieceBlack(unsigned piece, Direction d);
	void printgame() const;

private:
	void updateGame();
	void initGame();
};

#endif /*LAYER_HPP_ */
