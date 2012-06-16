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
	bool movePiece(unsigned piece, Direction d)
	{
		if (col == BLACK) 
			return movePieceBlack(piece,d);
		else 
			return movePieceRed(piece,d);
	}
	bool movePieceRed(unsigned piece, Direction d);
	bool movePieceBlack(unsigned piece, Direction d);
	void printgame() const;

private:
	void updateGame();
	void initGame();
};

#endif /*LAYER_HPP_ */
