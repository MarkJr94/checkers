/*
 * TranspositionalTable.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "TranspositionTable.hpp"
#include "AI.hpp"

/* Sleep portability thing */
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>

inline void delay( unsigned long ms )
{
	Sleep( ms );
}

#else  /* presume POSIX */

#include <unistd.h>

inline void delay(unsigned long ms) {
	usleep(ms * 1000);
}

#endif

unsigned TranspositionTable::tableSz = 100000000;

TT::TranspositionTable() :
		entries(tableSz) {

}

TT::~TranspositionTable() {
}

bool TT::hasEvaluated(const Hash::Zkey z) {

	int bucket = z % tableSz;
	return entries[bucket].done;
}
