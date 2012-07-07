#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

//~ std::vector<std::vector<Piece> > Player::game
									//~ (8,std::vector<Piece> (8));

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
	
	if (pieces[0] == NULL) return;
	
	cout << "Player color "  << (col == Piece::BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		if (p->getInPlay()) p->print();
	}
}
