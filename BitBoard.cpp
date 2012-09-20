/*
 * BitBoard.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: msinclair94
 */

#include "BitBoard.hpp"

namespace Bit {

namespace Masks {
const BitBoard S[32] = {
		 1u << 18, 1u << 12, 1u << 6, 1u << 0,
		 1u << 19, 1u << 13, 1u << 7, 1u << 1,
		 1u << 26, 1u << 20, 1u << 14, 1u << 8,
		 1u << 27, 1u << 21, 1u << 15, 1u << 9,
		 1u << 2, 1u << 28, 1u << 22, 1u << 16,
		 1u << 3, 1u << 29, 1u << 23, 1u << 17,
		 1u << 10, 1u << 4, 1u << 30, 1u << 24,
		 1u << 11, 1u << 05, 1u << 31, 1u << 25};

const BitBoard BP_INIT = S[0] | S[1] | S[2] | S[3] | S[4] | S[5] | S[6] | S[7]
		| S[8] | S[9] | S[10] | S[11];
const BitBoard WP_INIT = S[20] | S[21] | S[22] | S[23] | S[24] | S[25] | S[26]
		| S[27] | S[28] | S[29] | S[30] | S[31];

const BitBoard ROW_1 = S[0] | S[1] | S[2] | S[3];
const BitBoard ROW_2 = S[4] | S[5] | S[6] | S[7];
const BitBoard ROW_7 = S[24] | S[25] | S[26] | S[27];
const BitBoard ROW_8 = S[28] | S[29] | S[30] | S[31];

const BitBoard CAN_UPLEFT = ~(S[0] | S[8] | S[16] | S[24] | ROW_8);
const BitBoard CAN_UPRIGHT = ~(S[7] | S[15] | S[23] | S[31] | ROW_8);
const BitBoard CAN_DOWNLEFT = ~(S[0] | S[8] | S[16] | S[24] | ROW_1);
const BitBoard CAN_DOWNRIGHT = ~(S[7] | S[15] | S[23] | S[31] | ROW_1);

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
}

}




