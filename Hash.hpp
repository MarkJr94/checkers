/*
 * Hash.hpp
 *
 *  Created on: Aug 27, 2012
 *      Author: markjr
 */

#ifndef HASH_HPP_
#define HASH_HPP_

namespace Hash {

typedef long unsigned Zkey;

Zkey rand64(void);

/* Singleton class for GameWide zobrist table and hashing*/
class ZobristTable {
public:
	static ZobristTable& instance();

	~ZobristTable();
	Zkey**& operator[](const int row) {
		return zobrist[row];
	}
	const Zkey* const * const & operator[](const int row) const {
		return zobrist[row];
	}

private:
	ZobristTable();
	ZobristTable(const ZobristTable&);
	void operator=(ZobristTable&);
	Zkey*** zobrist;
};

}

#endif /* HASH_HPP_ */
