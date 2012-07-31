#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

typedef Piece::Color Color;

/* Constructor */
Player::Player(const Color color) :
		pieces(12, NULL), col(color), numPieces(12) {
}

/* Display player */
void Player::display() const {
	using namespace std;

	cout << "Player color " << (col == Piece::BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		if (p == NULL) {
			continue;
		}
		if (p->inPlay)
			p->print();
	}
}

Player::~Player() {
	for (auto p : pieces) {
		if (p != NULL) {
			if (p->id == ~0u) {
				delete p;
			}
		}
	}
}
