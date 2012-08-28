/*
 * Save.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#include "Save.hpp"

Save::Save() :
		turn(true), mustJump(0), data(8, std::vector<CellRecord>(8)) {
	using namespace std;

	unsigned i = 0, j = 0, count = 0;

	for (char k = 1; k <= 12; k++) {
		data[j][i] = {true, Piece::BLACK, k, false};
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
		data[j][i] = {true, Piece::RED, k, false};
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

Save::Save(bool turn) :
		turn(turn), mustJump(0), data(8, std::vector<CellRecord>(8)) {
}

//Save& Save::operator=(const Save& other) {
//	for (unsigned i = 0; i < BOARD_SIZE; i++) {
//		for (unsigned j = 0; j < BOARD_SIZE; j++) {
//			data[i][j] = other.data[i][j];
//		}
//	}
//	this->turn = other.turn;
//	this->mustJump = other.mustJump;
//	return *this;
//}

Save::~Save() {

}

void Save::write(std::string fname) {
	using namespace std;

	fstream savefile;
	savefile.open(fname.c_str(), fstream::trunc | fstream::out);

	savefile << turn << " " << mustJump << endl;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile << data[i][j].id << " " << data[i][j].alive << " "
					<< data[i][j].color << " " << data[i][j].isKing << " ";
		}
		savefile << endl;
	}
	savefile.close();
}
void Save::read(std::string fname) {
	using namespace std;

	fstream savefile;
	savefile.open(fname.c_str(), fstream::in);
	bool turn, alive, isKing;
	unsigned col, mj;
	short id;

	savefile >> turn >> mj;
	this->turn = turn;
	this->mustJump = mj;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile >> id >> alive >> col >> isKing;
			data[i][j].color = (Piece::Color) col;
			data[i][j].alive = alive;
			data[i][j].id = id;
			data[i][j].isKing = isKing;
		}
	}
	savefile.close();
}
