/*
 * Hash.hpp
 *
 *  Created on: Aug 27, 2012
 *      Author: markjr
 */

#ifndef HASH_HPP_
#define HASH_HPP_

#include "checkers.hpp"

namespace Hash {
typedef uint64_t Zkey;
Zkey rand64(void)
{
    return rand() ^ ((Zkey)rand() << 15) ^ ((Zkey)rand() << 30) ^
        ((Zkey)rand() << 45) ^ ((Zkey)rand() << 60);
}

class Zobrist {
public:
	Zobrist();

	Zkey** operator[](unsigned row) { return zobrist[row];}
	const Zkey** operator[](unsigned row) const { return zobrist[row];}

private:
	Zkey zobrist [2][2][64];
	static bool done;
};

}
class Hash {
public:
	Hash();
	virtual ~Hash();
};

#endif /* HASH_HPP_ */
