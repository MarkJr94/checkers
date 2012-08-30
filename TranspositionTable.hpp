/*
 * TranspositionalTable.h
 *
 *  Created on: Aug 29, 2012
 *      Author: msinclair94
 */
#pragma once
#ifndef TRANSPOSITIONTABLE_HPP_
#define TRANSPOSITIONTABLE_HPP_

#include <vector>

#include "Piece.hpp"
#include "Hash.hpp"

class AI;

typedef class TranspositionTable {
public:
	friend class AI;
	TranspositionTable();
	~TranspositionTable();

	TableEnt& operator[](const int elem) { return entries[elem];}

	const TableEnt& operator[](const int elem) const {return entries[elem];}

	bool hasEvaluated(const Hash::Zkey);

private:
	std::vector<TableEnt> entries;

	static unsigned tableSz;
} TT;

#endif /* TRANSPOSITIONTABLE_HPP_ */
