/*
 * TranspositionalTable.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "TranspositionTable.hpp"

unsigned TranspositionTable::tableSz = 100000000;

TT::TranspositionTable() :
		entries(tableSz) {

}

TT::~TranspositionTable() {
}

bool TT::hasEvaluated(const Hash::Zkey z) {

	return entries[z % tableSz].done;
}
