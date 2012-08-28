#include <iostream>

#include "Piece.hpp"

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

void Piece::print(std::ostream& out) const
{
	out << "id = " << id << " x = " << x << " y = " << y << " isKing = "
			<< isKing << " inplay = " << inPlay << " col = "
			<< (col == BLACK ? "Black" : "Red") << std::endl;
}
