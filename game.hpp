#ifndef GAME_HPP_
#define GAME_HPP_

#include "checkers.hpp"
#include "player.hpp"
#include <vector>

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

class Match {
	Player p1;
	Player p2;
	std::vector<std::vector<Piece> > board;
	/* Tracks if it's P1's turn or not */
	bool turn;
	bool debug;
	
public:
	/* Enumerations for movement and jump directions */
	enum Direction {LEFT,RIGHT};
	/* Constructor */
	Match(bool db = true);
	/* Constructor from memory */
	Match(SaveGame record, bool db);
	/* Print game */
	void print() const;
	/* Piece Movement */
	bool movePiece(unsigned piece, Direction d);
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey);
	/* Play */
	void play();
	/* Receive input for CLI */
	int receiveInput(unsigned piece, Direction d);
};

#endif /* GAME_HPP_ */
