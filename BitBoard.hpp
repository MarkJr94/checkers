#pragma once
#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <string>

const unsigned BOARD_SIZE = 8;

typedef unsigned BitBoard;

namespace Mask {

extern BitBoard DEST_L3, DEST_L5, DEST_R3, DEST_R5;
extern BitBoard WP_INIT, BP_INIT;
extern BitBoard ROW_2, ROW_7;
extern BitBoard ROW_1, ROW_8;

extern BitBoard S[32];

int bitCount(BitBoard v);

struct MaskInit {
	friend const MaskInit& inst();

	~MaskInit() {
	}
private:
	MaskInit() {
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

		ROW_1 = S[0] | S[1] | S[2] | S[3];
		ROW_2 = S[4] | S[5] | S[6]| S[7];
		ROW_7 = S[24] | S[25] | S[26] | S[27];
		ROW_8 = S[28] | S[29] | S[30] | S[31];
	}

	MaskInit(const MaskInit&);
	MaskInit& operator=(const MaskInit&);
};

const MaskInit& inst();
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
