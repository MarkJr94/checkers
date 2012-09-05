#pragma once
#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

#include <iosfwd>
#include <string>

const unsigned BOARD_SIZE = 8;

typedef unsigned BitBoard;

namespace BB {

BitBoard MASK_L3, MASK_L5, MASK_R3, MASK_R5;
BitBoard S[32];

void initMasks() {
	S[0] = 1;
	for (int i = 1; i < 32; i++) {
		S[i] = S[i - 1] * 2;
	}

	MASK_L3 = S[1] | S[2] | S[3] | S[9] | S[10] | S[11] | S[17] | S[18] | S[19]
			| S[25] | S[26] | S[27];
	MASK_L5 = S[4] | S[5] | S[6] | S[12] | S[13] | S[14] | S[20] | S[21]
			| S[22];
	MASK_R3 = S[28] | S[29] | S[30] | S[20] | S[21] | S[22] | S[12] | S[13]
			| S[14] | S[4] | S[5] | S[6];
	MASK_R5 = S[25] | S[26] | S[27] | S[17] | S[18] | S[19] | S[9] | S[10]
			| S[11];
}
}

//struct Piece {
//	enum Color {
//		BLACK, RED
//	};
//
//	bool inPlay;
//	bool isKing;
//	unsigned x;
//	unsigned y;
//	Color col;
//	unsigned id;
//
//	Piece(unsigned idNo = 0, unsigned xx = 0, unsigned yy = 0, Color color =
//			BLACK);
//
//	void print(std::ostream& out) const;
//};

struct Coord {
	unsigned short x;
	unsigned short y;
};

struct Move {
	Coord src;
	Coord dst;
};

enum Cell {
	EMPTY, P_RED, P_BLACK, K_RED, K_BLACK
};

const std::string _pptable[] { "---", "RED", "BLK", "KRD", "KBK" };

enum MoveCode {
	SUCCESS, OBSTRUCTED, ILLEGAL, WRONG_PIECE
};

const std::string _errtable[] { "SUCCESS", "OBSTRUCTED", "ILLEGAL",
		"WRONG_PIECE" };

enum Col {
	A, B, C, D, E, F, G, H
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
