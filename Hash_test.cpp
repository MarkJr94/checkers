/*
 * Hash_test.cpp
 *
 *  Created on: Aug 28, 2012
 *      Author: msinclair94
 */

#include "Hash.hpp"
#include <optparser>
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
	using namespace Hash;
	using namespace std;

	OP::OptParser parser("hash_test");
	parser.addOpt<int>("iters", 1, 'i', false,
			"Number of times to test Zobrist Table");
	parser.addOpt<bool>("help", false, 'h', false, "Display this help message");
	parser.parse(argc, argv);

	int iters = parser.getOpt<int>("iters");
	bool help = parser.getOpt<bool>("help");

	if (help) {
		parser.help();
		return 0;
	}

	for (int it = 0; it < iters; it++) {
		ZobristTable& z = ZobristTable::instance();
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 64; k++)
					cout << z[i][j][k] << " ";
		cout << "\n\n\n\n";
	}
}
