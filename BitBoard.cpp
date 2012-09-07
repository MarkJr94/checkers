/*
 * BitBoard.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: msinclair94
 */

#include "BitBoard.hpp"

namespace Mask {

 BitBoard S[32] = { 0x1, 0x2, 0x4, 0x8,
		 0x10, 0x20, 0x40, 0x80,
		 0x100, 0x200, 0x400, 0x800,
		 0x1000,0x2000,0x4000,0x8000,
 	 	 0x10000,0x20000,0x40000,0x80000,
 	 	 0x100000,0x200000,0x400000,0x800000,
 	 	 0x1000000,0x2000000,0x4000000,0x8000000,
 	 	 0x10000000,0x20000000,0x40000000,0x80000000};

 BitBoard	DEST_L3 = S[1] | S[2] | S[3] | S[9] | S[10] | S[11] | S[17] | S[18]
			| S[19] | S[25] | S[26] | S[27];
 BitBoard	DEST_L5 = S[4] | S[5] | S[6] | S[12] | S[13] | S[14] | S[20] | S[21]
			| S[22];
 BitBoard	DEST_R3 = S[28] | S[29] | S[30] | S[20] | S[21] | S[22] | S[12] | S[13]
			| S[14] | S[4] | S[5] | S[6];
 BitBoard	DEST_R5 = S[25] | S[26] | S[27] | S[17] | S[18] | S[19] | S[9] | S[10]
			| S[11];

 BitBoard	BP_INIT = 0x00000fff;
 BitBoard	WP_INIT = 0xfff00000;

 BitBoard	ROW_1 = S[0] | S[1] | S[2] | S[3];
 BitBoard	ROW_2 = S[4] | S[5] | S[6]| S[7];
 BitBoard	ROW_7 = S[24] | S[25] | S[26] | S[27];
 BitBoard	ROW_8 = S[28] | S[29] | S[30] | S[31];
 BitBoard	ROW_EVEN = 0x0f0f0f0f;
 BitBoard	ROW_ODD = 0xf0f0f0f0;


 std::map<BitBoard,unsigned short> bbUMap  = {
		{S[0],0},
		{S[1],1},
		{S[2],2},
		{S[3],3},
		{S[4],4},
		{S[5],5},
		{S[6],6},
		{S[7],7},
		{S[8],8},
		{S[9],9},
		{S[10],10},
		{S[11],11},
		{S[12],12},
		{S[13],13},
		{S[14],14},
		{S[15],15},
		{S[16],16},
		{S[17],17},
		{S[18],18},
		{S[19],19},
		{S[20],20},
		{S[21],21},
		{S[22],22},
		{S[23],23},
		{S[24],24},
		{S[25],25},
		{S[26],26},
		{S[27],27},
		{S[28],28},
		{S[29],29},
		{S[30],30},
		{S[31],31}
 };

 int bitCount(BitBoard v) {
 	BitBoard c;
 	v = v - ((v >> 1) & 0x55555555); // reuse input as temporary
 	v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // temp
 	c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
 	return c;
 }

}




