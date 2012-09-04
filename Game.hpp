#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "Piece.hpp"
#include "Save.hpp"
#include "Hash.hpp"

#include <map>
#include <string>
#include <vector>

class AI;

class Game {
public:
	friend class AI;
	friend class GameMaster;

	/* Constructor */
	Game(const bool db, const bool interact);
	/* Constructor from memory */
	Game(const Save& record, const bool db, const bool interact);
	/* Destructor */
	virtual ~Game();

	/* return save game */
	Save getSave();
	/* Print game */
	void print();

	/* Piece Movement */
	bool movePiece(const unsigned piece, const  Direction d);
	/* Jumping */
	bool jumpPiece(const unsigned jumper, const unsigned prey);
	/* Make a move from a MoveRecord */
	bool makeMove(const MoveRecord&);
	/* restore game to save */
	void restoreToSave(const Save& record);
	/* Receive input for CLI */
	int receiveInput();

	int getMustJump() const {
		return _mustJump;
	}

	void setMustJump(const int mustJump) {
		this->_mustJump = mustJump;
	}
	/* Get p1 score */
	unsigned getP1score() {
		return _p1.size();
	}
	/* Get p2 score */
	unsigned getP2score() {
		return _p2.size();
	}
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		_turn = newval;
	}
	bool getTurn() const {
		return _turn;
	}
	std::vector<Piece>& operator[](unsigned row) {
		return board[row];
	}
	const std::vector<Piece>& operator[](unsigned row) const {
		return board[row];
	}

	Hash::Zkey getHash() const;


protected:
	std::vector<std::vector<Piece> > board;

private:
	std::map <int,Piece *> _p1;
	std::map <int,Piece *> _p2;
	/* Tracks if it's P1's turn or not */
	bool _turn;
	bool _debug;
	Save _save;
	bool _interact;
	unsigned _mustJump;

	static Save templateSave;

	/* Update save game */
	void updateSave();
};

#endif /* GAME_HPP_ */
