/*
 * Save.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#include "Save.hpp"
#include <algorithm>

Save::Save() :
		turn(true), mustJump(0), _data(new Cell*[BOARD_SIZE]) {
	using namespace std;

	for (int i = 0; i < BOARD_SIZE; i++)
		_data[i] = new Cell[BOARD_SIZE];

	unsigned i = 0, j = 0, count = 0;

	for (char k = 1; k <= 12; k++) {
		_data[j][i] = P_BLACK;
		j += 2;
		if (j == BOARD_SIZE)
			j = 1;
		if (j == BOARD_SIZE + 1)
			j = 0;
		if (++count == 4) {
			++i;
			count = 0;
		}
	}

	count = 0;
	i = 7, j = 1;

	for (char k = 1; k <= 12; k++) {
		_data[j][i] = P_RED;
		j += 2;
		if (j == BOARD_SIZE)
			j = 1;
		if (j == BOARD_SIZE + 1)
			j = 0;
		if (++count == 4) {
			--i;
			count = 0;
		}
	}
}

Save::Save(const Save& other) :
		turn(other.turn), mustJump(other.mustJump), _data(
				new Cell*[BOARD_SIZE]) {
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		_data[i] = new Cell[BOARD_SIZE];
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			_data[i][j] = other._data[i][j];
		}
	}
}

void swap(Save& first, Save& second) {
	using std::swap;

	swap(first.turn, second.turn);
	swap(first.mustJump, second.mustJump);
	swap(first._data, second._data);
}

Save& Save::operator=(Save other) {

	swap(*this, other);

	return *this;
}

Save::~Save() {
	for (int i = 0; i < BOARD_SIZE; i++)
		delete []_data[i] ;
	delete [] _data;
}

void Save::write(std::string fname) const {
	using std::endl;
	using std::ofstream;

	ofstream savefile(fname.c_str());

	savefile << turn << " " << mustJump << endl;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile << _data[i][j] << " ";
		}
		savefile << endl;
	}
}

void Save::read(std::string fname) {
	using namespace std;

	fstream savefile(fname.c_str(), fstream::in);
	unsigned t, mj;
	int c;

	savefile >> t >> mj;
	turn = t;
	mustJump = mj;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile >> c;
			_data[i][j] = (Cell)c;
		}
	}
}
