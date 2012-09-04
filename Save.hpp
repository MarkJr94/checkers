/*
 * Save.hpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#ifndef SAVE_HPP_
#define SAVE_HPP_

#include "Piece.hpp"


#include <fstream>
#include <map>
#include <string>
#include <vector>


class Save {

public:
	bool turn;
	unsigned mustJump;
	unsigned char p1Score;
	unsigned char p2Score;

	Save();
	Save(const Save&);
	~Save();

	Save& operator=(Save);
	friend void swap(Save&,Save&);

	/* Equality test */
	bool operator==(const Save& other);

	/* Subscript operator */
	Cell* operator[](const int row) {
		return _data[row];
	}

	const Cell* operator[](const int row) const {
		return _data[row];
	}

	void write(std::string) const;
	void read(std::string);

private:
	Cell** _data;
};

#endif /* SAVE_HPP_ */
