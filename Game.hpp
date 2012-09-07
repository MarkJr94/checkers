#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "BitBoard.hpp"
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
	void print() const;

	/* Piece Movement */
	MoveCode makeMove(const Move&);

	/* Jumping */
	MoveCode jump(const Move&);
	/* restore game to save */
	void restoreToSave(const Save& record);
	/* Receive input for CLI */
	MoveCode receiveInput();

	/* Get p1 score */
	unsigned getP1score() const {
		return Mask::bitCount(_BP) + 2 * Mask::bitCount(_BP & _K);
	}
	/* Get p2 score */
	unsigned getP2score() const {
		return Mask::bitCount(_WP) + 2 * Mask::bitCount(_WP & _K);
	}
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		_turn = newval;
	}
	bool getTurn() const {
		return _turn;
	}

	Cell* toArr() const;

protected:
	BitBoard _WP;
	BitBoard _BP;
	BitBoard _K;

private:
	typedef BitBoard BB;
	/* Tracks if it's P1's turn or not */
	bool _turn;
	bool _debug;
	Save _save;
	bool _interact;
	unsigned char _mustJump;

	/* Update save game */
	void updateSave();


	BitBoard getJumpers() const;
	BitBoard getMovers() const;
	BitBoard getEmpty() const {
		return ~(_WP | _BP);
	}
};

#endif /* GAME_HPP_ */
