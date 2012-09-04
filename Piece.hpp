#pragma once
#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

#include <iosfwd>
#include <string>

const unsigned BOARD_SIZE = 8;

//struct Piece {
//	enum Color {
//		BLACK, RED
//	};
//
//	bool inPlay;
//	bool isKing;
//	unsigned x;
//	unsigned y;
//	Color col;
//	unsigned id;
//
//	Piece(unsigned idNo = 0, unsigned xx = 0, unsigned yy = 0, Color color =
//			BLACK);
//
//	void print(std::ostream& out) const;
//};

struct Coord {
	unsigned x;
	unsigned y;
};

struct Move {
	Coord src;
	Coord dst;
};

enum Cell { EMPTY, P_RED, P_BLACK, K_RED, K_BLACK};

const std::string _pptable[] { "---", "RED", "BLK", "KRD", "KBK" };

enum MoveCode { SUCCESS, OBSTRUCTED, ILLEGAL, WRONG_PIECE };

const std::string _errtable[] { "SUCCESS", "OBSTRUCTED", "ILLEGAL", "WRONG_PIECE"};

enum Col { A, B, C, D, E, F, G, H};

enum Direction {
	LEFT, RIGHT, BKLEFT, BKRIGHT
};

struct MoveRecord {
	Direction dir;
	bool jump;
	unsigned piece;
	unsigned prey;
};

#endif /* CHECKERS_HPP_ */
