#include <iostream>

#include "checkers.hpp"
/* Piece constructor:
 * If any value is out of range, it is automatically set to 0.
 */
Piece::Piece(unsigned idNo, unsigned xx, unsigned yy, Piece::Color color)
: inPlay (false), isKing (false), x (xx), y (yy), col (color), id (idNo)
{
	if (idNo > 12 && idNo != ~0u) {
		id = 0;
	} else {
		id = idNo;
	}

	if (xx > 7) {
		x = 0;
	} else {
		x = xx;
	}

	if (yy > 7) {
		y = 0;
	} else {
		y = yy;
	}
}
//
//unsigned Piece::getX() const {return x;}
//
//unsigned Piece::getY() const {return y;}
//
//bool Piece::getIsKing() const {return isKing;}
//
//bool Piece::getInPlay() const {return inPlay;}
//
//Piece::Color Piece::getColor() const {return col;}

void Piece::print() const
{
	std::cout << "id = " << id << " x = " << x << " y = " << y << " isKing = "
			<< isKing << " inplay = " << inPlay << " col = "
			<< (col == BLACK ? "Black" : "Red") << std::endl;
}

//void Piece::setX(const unsigned xx)
//{
//	if (xx > 7) {
//		std::cerr << "Invalid x value\n";
//		return;
//	}
//	x = xx;
//}
//
//void Piece::setY(const unsigned yy) {
//	if (yy > 7) {
//		std::cerr << "Invalid y value\n";
//		return;
//	}
//	y = yy;
//}
//
//void Piece::setIsKing(const bool newval) {isKing = newval;}
//
//void Piece::setInPlay(const bool newval) {inPlay = newval;}
//
//void Piece::setColor(const Color color) {col = color;}
