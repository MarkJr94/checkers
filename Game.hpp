#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "Piece.hpp"
#include "Save.hpp"
#include "Hash.hpp"
#include "TranspositionTable.hpp"

#include <map>
#include <string>
#include <vector>

class AI;

class Game {
public:
	friend class AI;
	friend class TranspositionTable;

	/* Constructor */
	Game(const bool db, const bool interact);
	/* Constructor from memory */
	Game(const Save& record, const bool db, const bool interact);
	/* Destructor */
	virtual ~Game();

	/* Update save game */
	void updateSave();
	/* return save game */
	Save getSave();
	/* Print game */
	void print();

	/* Piece Movement */
	bool movePiece(const unsigned piece, const  Direction d);
	/* Jumping */
	bool jumpPiece(const unsigned jumper, const unsigned prey);
	/* restore game to save */
	void restoreToSave(const Save& record);
	/* Receive input for CLI */
	int receiveInput();

	int getMustJump() const {
		return mustJump;
	}

	void setMustJump(const int mustJump) {
		this->mustJump = mustJump;
	}
	/* Get p1 score */
	unsigned getP1score() {
		return p1.size();
	}
	/* Get p2 score */
	unsigned getP2score() {
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

	Hash::Zkey getHash() const { return hash; }

protected:
	std::vector<std::vector<Piece> > board;

private:
	std::map <int,Piece *> p1;
	std::map <int,Piece *> p2;
	/* Tracks if it's P1's turn or not */
	bool turn;
	bool debug;
	Save save;
	bool interact;
	unsigned mustJump;
	Hash::Zkey hash;

	static Save templateSave;
};

#endif /* GAME_HPP_ */
