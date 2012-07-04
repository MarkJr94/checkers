#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_
#include <vector>

class Piece {
public:
	enum Color { BLACK, RED};

private:
	bool inPlay;
	bool isKing;
	unsigned x;
	unsigned y;
	Color col;
	
public:
	unsigned id;

	Piece(unsigned idNo = 0, unsigned xx = 0, unsigned yy = 0, Color color = BLACK);
	unsigned getX() const;
	unsigned getY() const;
	bool getInPlay() const;
	bool getIsKing() const;
	Color getColor() const;
	
	Piece & operator=(const Piece& other);
	
	void print() const;
	

public:
	void setX(const unsigned);
	void setY(const unsigned);
	void setIsKing(const bool newval = true);
	void setInPlay(const bool newval = true);
	void setColor(const Color);
	bool obstructedAt(unsigned xx, unsigned yy, const Piece other);
};

#endif /* CHECKERS_HPP_ */
