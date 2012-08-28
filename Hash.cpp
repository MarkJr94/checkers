/*
 * Hash.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: markjr
 */

#include "Hash.hpp"

#include <cstdlib>
#include <ctime>

Hash::Zkey Hash::rand64(void) {
	return rand() ^ ((Zkey) rand() << 15) ^ ((Zkey) rand() << 30)
			^ ((Zkey) rand() << 45) ^ ((Zkey) rand() << 60);
}

Hash::ZobristTable::ZobristTable() {
	zobrist = new Zkey**[2];
	srand(time(NULL));

	for (int i = 0; i < 2; i++) {
		zobrist[i] = new Zkey*[2];
		for (int j = 0; j < 2; j++) {
			zobrist[i][j] = new Zkey[64];
			for (int k = 0; k < 64; k++)
				zobrist[i][j][k] = rand64();
		}
	}
}

Hash::ZobristTable::~ZobristTable() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			delete[] zobrist[i][j];
		}
		delete[] zobrist[i];
	}
	delete[] zobrist;
}

Hash::ZobristTable& Hash::ZobristTable::instance() {
	static ZobristTable z;

	return z;
}
