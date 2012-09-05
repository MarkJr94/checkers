#pragma once
#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

#include <iosfwd>
#include <string>

const unsigned BOARD_SIZE = 8;

typedef unsigned BitBoard;

struct Masks {

	BitBoard DEST_L3, DEST_L5, DEST_R3, DEST_R5;
	BitBoard WP_INIT, BP_INIT;

	BitBoard S[32];

	static const Masks& inst() {
		static Masks b;
		return b;
	}

	static const inline int bitCount(BitBoard v) {
		BitBoard c;
		v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
		v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
		c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
		return c;
	}

	~Masks() {}

private:
	Masks() {
		for (int i = 0; i < 32; i++) {
			S[i] = 1 << i;
		}

		DEST_L3 = S[1] | S[2] | S[3] | S[9] | S[10] | S[11] | S[17] | S[18]
				| S[19] | S[25] | S[26] | S[27];
		DEST_L5 = S[4] | S[5] | S[6] | S[12] | S[13] | S[14] | S[20] | S[21]
				| S[22];
		DEST_R3 = S[28] | S[29] | S[30] | S[20] | S[21] | S[22] | S[12] | S[13]
				| S[14] | S[4] | S[5] | S[6];
		DEST_R5 = S[25] | S[26] | S[27] | S[17] | S[18] | S[19] | S[9] | S[10]
				| S[11];

		BP_INIT = 0x00000fff;
		WP_INIT = 0xfff00000;
	}

	Masks(const Masks&);
	Masks& operator=(const Masks&);
};


struct Move {
	unsigned short src;
	unsigned short dst;
};

enum Cell { EMPTY, P_W, P_B, K_W, K_B};

const std::string _ctable[] { "---", "WHI", "BLK", "KWH", "KBK"
};

enum MoveCode {
	SUCCESS, OBSTRUCTED, ILLEGAL, WRONG_PIECE
};

const std::string _errtable[] { "SUCCESS", "OBSTRUCTED", "ILLEGAL",
		"WRONG_PIECE" };

enum Col {
	A, B, C, D, E, F, G, H
};

#endif /* CHECKERS_HPP_ */
