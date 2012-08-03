#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "checkers.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct cellRecord {
	bool alive;
	Piece::Color color;
	unsigned id;
	bool isKing;
};

struct SaveGame {

	bool turn;
	unsigned mustJump;

	SaveGame(bool turn);
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		turn = newval;
	}
	bool getTurn() {
		return turn;
	}
	/* Assignment operator */
	SaveGame & operator=(const SaveGame other);

	std::vector<cellRecord> & operator[](int row) {
		return data[row];
	}

	void write(std::string fname);
	void read(std::string fname);

	unsigned getMustJump() const {
		return mustJump;
	}

	void setMustJump(unsigned mustJump) {
		this->mustJump = mustJump;
	}

private:
	std::vector<std::vector<cellRecord>> data;
};

class Game {


public:
	/* Enumerations for movement and jump directions */
	enum Direction {
		LEFT, RIGHT, BKLEFT, BKRIGHT
	};

	/* Constructor */
	Game(bool db, bool interact);
	/* Constructor from memory */
	Game(SaveGame record, bool db, bool interact);
	/* Destructor */
	virtual ~Game() {
	}
	/* Update save game */
	inline void updateSave();
	/* return save game */
	SaveGame getSave();
	/* Print game */
	void print();
	/* Piece Movement */
	bool movePiece(unsigned piece, Direction d);
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey);
	/* restore game to save */
	void restoreToSave(SaveGame& record);
	/* Receive input for CLI */
	int receiveInput();

	int getMustJump() const {
		return mustJump;
	}

	void setMustJump(int mustJump) {
		this->mustJump = mustJump;
	}
	/* Get p1 score */
	inline unsigned getP1score() {
		return p1.size();
	}
	/* Get p2 score */
	inline unsigned getP2score() {
		return p2.size();
	}
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		turn = newval;
	}
	bool getTurn() const {
		return turn;
	}
	std::vector<Piece>& operator[](unsigned row) {
		return board[row];
	}
	const std::vector<Piece>& operator[](unsigned row) const {
		return board[row];
	}

private:
	std::map <int,Piece *> p1;
	std::map <int,Piece *> p2;
	std::vector<std::vector<Piece> > board;
	/* Tracks if it's P1's turn or not */
	bool turn;
	bool debug;
	SaveGame save;
	bool interact;
	unsigned mustJump;


	virtual void _print();
};

#endif /* GAME_HPP_ */
