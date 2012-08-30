#pragma once
#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

#include <iosfwd>

const unsigned BOARD_SIZE = 8;

struct Piece {
	enum Color {
		BLACK, RED
	};

	bool inPlay;
	bool isKing;
	unsigned x;
	unsigned y;
	Color col;
	unsigned id;

	Piece(unsigned idNo = 0, unsigned xx = 0, unsigned yy = 0, Color color =
			BLACK);

	void print(std::ostream& out) const;
};

enum Direction {
	LEFT, RIGHT, BKLEFT, BKRIGHT
};

struct MoveRecord {
	Direction dir;
	bool jump;
	unsigned piece;
	unsigned prey;
};

#endif /* CHECKERS_HPP_ */
