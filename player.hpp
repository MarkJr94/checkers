#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "checkers.hpp"



class Player {
	std::vector<Piece> pieces;
	Piece::Color col;

public:
	static std::vector<std::vector<Piece> > game;
	unsigned numPieces;
	
	/* Enumerations for movement and jump directions */
	enum Direction {LEFT, RIGHT};
	enum JumpDi {UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};
	
private:
	bool movePieceRed(unsigned piece, Direction d);
	bool movePieceBlack(unsigned piece, Direction d);
	bool redJump(unsigned jumper, unsigned prey, Player& other);
	bool blackJump(unsigned jumper, unsigned prey, Player& other);

public:
	Player(const Piece::Color);
	void display() const;
	
	/*Wrapper for private move functions for different colors */
	bool movePiece(unsigned piece, Direction d)
	{
		if (col == Piece::BLACK) 
			return movePieceBlack(piece,d);
		return movePieceRed(piece,d);
	}
	
	/* Wrapper for private jump functions for different colors */
	bool jumpPiece(unsigned jumper, unsigned prey, Player& other)
	{
		if (col == Piece::BLACK)
			return blackJump(jumper, prey, other);
		return redJump(jumper,prey,other);
	}
	void printgame() const;

private:
	//~ bool movePieceRed(unsigned piece, Direction d);
	//~ bool movePieceBlack(unsigned piece, Direction d);
	//~ bool redJump(unsigned jumper, unsigned prey, Player& other);
	//~ bool blackJump(unsigned jumper, unsigned prey, Player& other);
	void updateGame();
	void initGame();
};

#endif /*LAYER_HPP_ */
