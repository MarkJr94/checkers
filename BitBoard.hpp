#pragma once
#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <string>
#include <map>
#include <iostream>

const unsigned BOARD_SIZE = 8;

typedef unsigned BitBoard;

typedef BitBoard BB;

namespace Mask {

extern BitBoard DEST_L3, DEST_L5, DEST_R3, DEST_R5;
extern BitBoard WP_INIT, BP_INIT;
extern BitBoard ROW_2, ROW_7;
extern BitBoard ROW_1, ROW_8;
extern BitBoard ROW_ODD, ROW_EVEN;

extern BitBoard S[32];

extern std::map<BitBoard,unsigned short> bbUMap;

int bitCount(BitBoard v);

inline BB numToBB(unsigned n) { return S[n]; }

inline BB highBit(BB board) {
	board |= (board >> 1);
	board |= (board >> 2);
	board |= (board >> 4);
	board |= (board >> 8);
	board |= (board >> 16);
	return board - (board >> 1);
}

}


struct Move {
	unsigned short src;
	unsigned short dst;
};

enum Cell {
	EMPTY, P_W, P_B, K_W, K_B
};

const std::string _ctable[] { "---", "WHI", "BLK", "KWH", "KBK" };

enum MoveCode {
	SUCCESS, OBSTRUCTED, ILLEGAL, WRONG_PIECE, QUIT, INPUT_FAIL
};

const std::string _errtable[] { "SUCCESS", "OBSTRUCTED", "ILLEGAL",
		"WRONG_PIECE", "QUIT", "INPUT_FAIL" };

enum Col {
	A, B, C, D, E, F, G, H
};

#endif /* BITBOARD_HPP_ */
