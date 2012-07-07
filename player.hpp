#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "checkers.hpp"

struct cellRecord {
	bool alive;
	Piece::Color color;
	unsigned id;
};

class SaveGame {
	std::vector<std::vector<cellRecord>> data;
	bool turn;
	
public:
	SaveGame(bool turn)
			: data (8, std::vector<cellRecord> (8)), turn(turn)
	{;}
	
	SaveGame & operator=(const SaveGame other)
	{
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			for (unsigned j = 0; j < BOARD_SIZE; j++) {
				data[i][j] = other.data[i][j];
			}
		}
		return *this;
	}

	cellRecord & operator()(int row, int col)
	{
		return data[row][col];
	}
	
	std::vector<cellRecord> & operator()(int row)
	{
		return data[row];
	}
};

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
	/* constructor from memory */
	Player (const Piece::Color, 
		const SaveGame record, bool db);
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
	/* get pointer to game */
	std::vector<std::vector<Piece> > *getGame();
	std::vector<Piece *> *getPieces() { return &pieces;}
};

#endif /*PLAYER_HPP_ */
