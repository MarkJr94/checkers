/*
 * Save.hpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */

#ifndef SAVE_HPP_
#define SAVE_HPP_

#include "BitBoard.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

struct Save {

	bool turn;
	BitBoard mustJump;
	BitBoard WP;
	BitBoard BP;
	BitBoard K;

	Save();
	~Save();
	friend void swap(Save&, Save&);

	/* Equality test */
	friend bool operator==(const Save&, const Save&);

	/* Read and write to file */
	void write(std::string) const;
	void read(std::string);

	int grade()
	{
		int p1 = Bit::bitCount(BP & ~K) + 0*2 * Bit::bitCount(BP & K);
		int p2 = Bit::bitCount(WP & ~K) + 0*2 * Bit::bitCount(WP & K);
		p2 *= -1;

		return p1 + p2;
	}

private:
};

#endif /* SAVE_HPP_ */
