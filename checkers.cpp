#include <iostream>

#include "checkers.hpp"

std::vector<std::vector<bool> > Piece::board 
									(8, std::vector<bool> (8,false));

Piece::Piece(unsigned idNo, unsigned xx, unsigned yy, Color color)
		: inPlay (true), isKing (false), x (xx), y (yy), col (color), id (idNo)
{
	if (idNo > 12) {
		id = 0;
		std::cerr << "id too large, set to 0\n";
	} else {
		id = idNo;
	}
	
	if (xx > 7) {
		x = 0;
		std::cerr << "x value too large, set to 0\n";
	} else {
		x = xx;
	}
	
	if (yy > 7) {
		y = 0;
		std::cerr << "y value too large, set to 0\n";
	} else {
		y = yy;
	}
	addToBoard();
}
		
unsigned Piece::getX() const {return x;}

unsigned Piece::getY() const {return y;}

bool Piece::getIsKing() const {return isKing;}

bool Piece::getInPlay() const {return inPlay;}

Color Piece::getColor() const {return col;}

Piece & Piece::operator=(const Piece& other)
{
	if (this != &other) {
		x = other.x;
		y = other.y;
		id = other.id;
		isKing = other.isKing;
		inPlay = other.inPlay;
		col = other.col;
	}
	
	return *this;
}

void Piece::addToBoard() const
{
	board[x][y] = true;
}

void Piece::removeFromBoard() const
{
	board[x][y] = false;
}
	

void Piece::print() const
{
	std::cout << "id = " << id << " x = " << x << " y = " << y << " isKing = "
		<< isKing << " inplay = " << inPlay << " col = "
		<< (col == BLACK ? "Black" : "Red") << std::endl;
}

void Piece::printBoard()
{
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i]) {
				std::cout << "O";
			} else {
				std::cout << "-";
			}
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Piece::setX(const unsigned xx) 
{
	if (xx > 7) {
		std::cerr << "Invalid x value\n";
		return;
	}
	x = xx; 
}

void Piece::setY(const unsigned yy) {
	if (yy > 7) {
		std::cerr << "Invalid y value\n";
		return;
	}
	y = yy;
}

void Piece::setIsKing(const bool newval) {isKing = newval;}

void Piece::setInPlay(const bool newval) {inPlay = newval;}

void Piece::setColor(const Color color) {col = color;}

bool Piece::obstructedAt(unsigned xx, unsigned yy, const Piece other)
{
	if (xx > 7 || yy > 7) {
		return true;
	}
	if (xx == other.x && yy == other.y) {
		return true;
	}
	return false;
}
