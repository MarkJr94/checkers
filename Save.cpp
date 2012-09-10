/*
 * Save.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#include "Save.hpp"
#include <algorithm>

Save::Save() :
		turn(true), mustJump(0), WP(Bit::Masks::WP_INIT), BP(Bit::Masks::BP_INIT), K(0) {

}

void swap(Save& first, Save& second) {
	using std::swap;

	swap(first.turn, second.turn);
	swap(first.mustJump, second.mustJump);
	swap(first.WP, second.WP);
	swap(first.BP, second.BP);
	swap(first.K, second.K);
}

Save::~Save() {
}

bool operator==(const Save& lhs, const Save& rhs) {
	return lhs.mustJump == rhs.mustJump && lhs.turn == rhs.turn && lhs.WP == rhs.WP
			&& lhs.BP == rhs.BP && lhs.K == rhs.K;
}

void Save::write(std::string fname) const {
	using std::endl;
	using std::ofstream;

	ofstream saveFile(fname.c_str());

	saveFile << turn << " " << mustJump << endl;
	saveFile << WP << endl;
	saveFile << BP << endl;
	saveFile << K << endl;
}

void Save::read(std::string fname) {
	using std::ifstream;

	ifstream saveFile(fname.c_str());

	saveFile >> turn >> mustJump;
	saveFile >> WP;
	saveFile >> BP;
	saveFile >> K;
}
