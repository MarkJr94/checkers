#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "Piece.hpp"
#include "Save.hpp"
#include "Hash.hpp"

#include <map>
#include <string>
#include <vector>
#include <utility>

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

//	/* Piece Movement */
	MoveCode makeMove(const Move&);
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
		return _p1Score;
	}
	/* Get p2 score */
	unsigned getP2score() {
		return _p2Score;
	}
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		_turn = newval;
	}
	bool getTurn() const {
		return _turn;
	}
	Cell* operator[](unsigned row) {
		return _board[row];
	}
	const Cell* operator[](unsigned row) const {
		return _board[row];
	}

	Hash::Zkey getHash() const;


protected:
	Cell** _board;
//	std::vector<std::vector<Piece> > board;

private:
//	std::map <int,Piece *> _p1;
//	std::map <int,Piece *> _p2;
	/* Tracks if it's P1's turn or not */
	bool _turn;
	bool _debug;
	Save _save;
	bool _interact;
	unsigned char _mustJump;
	unsigned char _p1Score;
	unsigned char _p2Score;

	static Save templateSave;

	/* Update save game */
	void updateSave();

	MoveCode moveBlack(const Move&);
	MoveCode moveRed(const Move&);
	MoveCode moveKBlack(const Move&);
	MoveCode moveKRed(const Move&);
};

#endif /* GAME_HPP_ */
