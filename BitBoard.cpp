/*
 * BitBoard.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: msinclair94
 */

#include "BitBoard.hpp"

namespace Mask {
 BitBoard DEST_L3, DEST_L5, DEST_R3, DEST_R5;
 BitBoard WP_INIT, BP_INIT;
 BitBoard ROW_2, ROW_7;
 BitBoard ROW_1, ROW_8;

 BitBoard S[32];

 int bitCount(BitBoard v) {
 	BitBoard c;
 	v = v - ((v >> 1) & 0x55555555); // reuse input as temporary
 	v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // temp
 	c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
 	return c;
 }

 const MaskInit& inst() {
 	static Mask::MaskInit m;
 	return m;
 }

}




