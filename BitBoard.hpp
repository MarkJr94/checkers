#pragma once
#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <string>
#include <map>
#include <iostream>
#include <limits>

const unsigned BOARD_SIZE = 8;

typedef unsigned BitBoard;

typedef BitBoard BB;

namespace Bit {

namespace Masks {

extern const BitBoard WP_INIT, BP_INIT;
extern const BitBoard ROW_2, ROW_7;
extern const BitBoard ROW_1, ROW_8;
extern const BitBoard CAN_UPLEFT, CAN_UPRIGHT;
extern const BitBoard CAN_DOWNLEFT, CAN_DOWNRIGHT;

extern const BitBoard S[32];

extern std::map<BitBoard,unsigned short> bbUMap;
}

inline int bitCount(BitBoard v) {
 	BitBoard c;
 	v = v - ((v >> 1) & 0x55555555); // reuse input as temporary
 	v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // temp
 	c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
 	return c;
}

inline BB highBit(BB board) {
	board |= (board >> 1);
	board |= (board >> 2);
	board |= (board >> 4);
	board |= (board >> 8);
	board |= (board >> 16);
	return board - (board >> 1);
}

template <typename T>
T inline rol(T v, unsigned n) {
	return (v << n) | (v >> (std::numeric_limits<T>::digits - n) );
}

template <typename T>
T inline ror(T v, unsigned n) {
	return (v >> n) | (v << (std::numeric_limits<T>::digits - n) );
}

}


struct Move {
	unsigned short src;
	unsigned short dst;
	bool jump;
};

enum Cell {
	EMPTY, P_W, P_B, K_W, K_B
};

const std::string cellTable[] { "---", "WHI", "BLK", "KWH", "KBK" };

enum MoveCode {
	SUCCESS, VOID_PIECE, ILLEGAL_MOVE, WRONG_PIECE, QUIT, INPUT_FAIL
};

const std::string errorTable[] { "SUCCESS", "VOID_PIECE", "ILLEGAL_MOVE",
		"WRONG_PIECE", "QUIT", "INPUT_FAIL" };

#endif /* BITBOARD_HPP_ */
