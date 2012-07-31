#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_
#include <vector>

const unsigned BOARD_SIZE = 8;

struct Piece {
	enum Color { BLACK, RED};

	bool inPlay;
	bool isKing;
	unsigned x;
	unsigned y;
	Color col;
	unsigned id;

//	unsigned getId() const {
//		return id;
//	}
//
//	void setId(unsigned id) {
//		this->id = id;
//	}
//
	Piece(unsigned idNo = 0, unsigned xx = 0, unsigned yy = 0, Color color = BLACK);
//	unsigned getX() const;
//	unsigned getY() const;
//	bool getInPlay() const;
//	bool getIsKing() const;
//	Color getColor() const;
//
//	Piece & operator=(const Piece& other);

	void print() const;

//	void setX(const unsigned);
//	void setY(const unsigned);
//	void setIsKing(const bool newval = true);
//	void setInPlay(const bool newval = true);
//	void setColor(const Color);
};

#endif /* CHECKERS_HPP_ */
