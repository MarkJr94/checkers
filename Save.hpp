/*
 * Save.hpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#ifndef SAVE_HPP_
#define SAVE_HPP_

#include "checkers.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct CellRecord {
	bool alive;
	Piece::Color color;
	unsigned id;
	bool isKing;
};

struct Save {

	bool turn;
	unsigned mustJump;

	Save();
	Save(bool);
	~Save();

	/* Assignment operator */
	Save& operator=(const Save&);

	/* Subscript operator */
	std::vector<CellRecord>& operator[](const int row) {
		return data[row];
	}

	const std::vector<CellRecord>& operator[](const int row) const {
		return data[row];
	}

	void write(std::string);
	void read(std::string);

private:
	std::vector<std::vector<CellRecord>> data;
};

#endif /* SAVE_HPP_ */
