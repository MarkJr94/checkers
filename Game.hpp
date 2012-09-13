#pragma once
#ifndef GAME_HPP_
#define GAME_HPP_

#include "BitBoard.hpp"
#include "Save.hpp"

#include <map>
#include <string>
#include <vector>
#include <utility>

class AI;

class Game {
public:
	friend class AI;
	friend class GameMaster;
	friend class SFMLGame;

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
		return Bit::bitCount(_BP & ~_K) + 2 * Bit::bitCount(_BP & _K);
	}
	/* Get p2 score */
	unsigned getP2score() const {
		return Bit::bitCount(_WP & ~_K) + 2 * Bit::bitCount(_WP & _K);
	}

	unsigned p1NumPieces() const {return Bit::bitCount(_BP);}
	unsigned p2NumPieces() const {return Bit::bitCount(_WP);}

	bool isLive () const { return _WP && _BP;}

	std::vector<Cell> toArr() const;

private:
	BitBoard _WP;
	BitBoard _BP;
	BitBoard _K;


	typedef BitBoard BB;
	/* Tracks if it's P1's turn or not */
	bool _turn;
	bool _debug;
	Save _save;
	bool _interact;
	BB _mustJump;

	/* Update save game */
	void updateSave();


	BitBoard getJumpers() const;
	BitBoard getMovers() const;
	BitBoard getEmpty() const {
		return ~(_WP | _BP);
	}
	inline BB canJump(const BB src, const BB vict);
};

#endif /* GAME_HPP_ */
