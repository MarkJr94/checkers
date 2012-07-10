#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

typedef Piece::Color Color;

/* Constructor */
Player::Player(const Color color, bool db)
		: pieces (12, NULL), col (color), numPieces(12), debug (db)
{
	;
}

/* Display player */
void Player::display() const
{
	using namespace std;
	
	cout << "Player color "  << (col == Piece::BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		if (p == NULL) {
			cerr << "NULL PIECESSSSSS!!!!\n";
			continue;
		}
		if (p->getInPlay())
			p->print();
	}
}

Player::~Player()
{
	for (auto p : pieces) {
		if (p->id == ~0u) {
			std::cout << "deleted by force\n";
			delete p;
		}
	}
}
