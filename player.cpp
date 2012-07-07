#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

std::vector<std::vector<Piece> > Player::game
									(8,std::vector<Piece> (8));

typedef Piece::Color Color;

/* Constructor */
Player::Player(const Color color, bool db)
		: pieces (12, NULL), col (color), numPieces(12), debug (db)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			game[i][j].setX(i);
			game[i][j].setY(j);
		}
	}
	int i = 0, j = 0, count = 0, id = 0;
	/* Initialize if piece is red */
	if (color == Piece::RED) {
		i = 7; j = 1;
		for (auto &q : pieces) {
			q = &game[j][i];
			Piece& p = game[j][i];
			q->setX(j); p.setY(i);
			q->setInPlay(true);
			q->setColor(color);
			q->setIsKing(false);
			q->id = id++;
			j += 2;
			if (j == 8) j = 1;
			if (j == 9) j = 0;
			if ((++count == 4))  {
				--i;
				count = 0;
			}
		}
	/* Initialize if piece is black */
	} else if (col == Piece::BLACK) {
		i = j = 0;
		for (auto &q : pieces) {
			q = &game[j][i];
			Piece& p = game[j][i];
			q->setX(j); p.setY(i);
			q->setInPlay(true);
			q->setColor(color);
			q->setIsKing(false);
			q->id = id++;
			j += 2;
			if (j == 8) j = 1;
			if (j == 9) j = 0;
			if ((++count == 4))  {
				++i;
				count = 0;
			}
			
		}
	} else {
		std::cerr << "Error in Player::Player: unknown color\n";
		exit(1);
	}
}

/* Constructor from memory */
Player::Player (Piece::Color color, SaveGame record, bool db)
		: pieces(12, NULL), col (color), debug (db)
{
	using namespace std;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			game[i][j].setX(i);
			game[i][j].setY(j);
		}
	}
	
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			if (record(i,j).color == col && record(i,j).alive) {
				game[i][j].setInPlay(true);
				game[i][j].setColor(col);
				game[i][j].id = record(i,j).id;
			}
		}
	}
}

/* Display player */
void Player::display() const
{
	using namespace std;
	
	cout << "Player color "  << (col == Piece::BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		if (p->getInPlay()) p->print();
	}
}

/* Get number of pieces */
unsigned Player::getnPieces() const
{
	return numPieces;
}

/* Piece movement */
bool Player::movePiece(unsigned piece, Direction d)
{
	using namespace std;
	auto &alias = pieces[piece];
	
	/* Testing if piece selection is valid */
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if (!alias->getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != alias->getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	if (debug) alias->print();
	if (debug) cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	
	/* Determine next coordinates for jump */
	unsigned nextx, nexty;
	
	if (col == Piece::RED)
		nexty = alias->getY() - 1;
	else
		nexty = alias->getY() + 1;
		
	if (d == LEFT)
		nextx = alias->getX() - 1;
	else
		nextx = alias->getX() + 1;
	
	/* Testing move validity */
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	
	if (nextx > 7) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	if (game[nextx][nexty].getInPlay()) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	
	/* Complete the move */
	alias->setInPlay(false);
	alias = &game[nextx][nexty];
	alias->setInPlay(true);
	alias->setColor(col);
	
	alias->id = piece;
	return true;
}

/* Print the game */
void Player::printgame() const
{
	for (int j = 7; j >= 0; j--) {
		for (int i = 0; i < 8; i++) {
			if (game[i][j].getInPlay()) {
				unsigned thisid = game[i][j].id;
				std::cout << (thisid < 10 ? "0" : "")<< thisid
					<< (game[i][j].getColor() == Piece::RED? "R" : "B");
			} else {
				std::cout << "---";
			}
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

std::vector<std::vector<Piece> > * Player::getGame()
{
	return &game;
}

/* Jumping */
bool Player::jumpPiece(unsigned jumper, unsigned prey, Player& other)
{
	using namespace std;
	Piece& j = *pieces[jumper];
	Piece& p = *other.pieces[prey];
	
	/* Testing if piece selection is valid */
	if ( jumper > 11 || prey > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if (!j.getInPlay() || !p.getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != j.getColor() || col == p.getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	if (debug) {j.print(); cout << "Preying on: "; p.print();}
	
	/* Testing if valid targets */
	if (col == Piece::RED) {
		if (p.getY() > j.getY()) {
			cerr << "Invalid target in Y direction\n";
			return false;
		}
	} else {
		if (p.getY() < j.getY()) {
			cerr << "Invalid target in Y direction\n";
			return false;
		}
	}
	int ydiff = (int)p.getY() - (int)j.getY();
	if (ydiff != 1 && ydiff != -1) {
		cerr << "Invalid target in Y direction\n";
		return false;
	}
	int diff = (int)p.getX() - (int)j.getX();
	if (diff != 1 && diff != -1) {
		cerr << "Invalid target in X direction\n";
		return false;
	}
	
	/* Testing the validity of the jump */
	unsigned newx = j.getX() + diff * 2;
	unsigned newy;
	if (col == Piece::RED) {
		newy = j.getY() - 2;
	} else {
		newy = j.getY() + 2;
	}
	if (newx > 7 || newy > 7) {
		cerr << "Jump obstructed at border\n";
		return false;
	}
	if (game[newx][newy].getInPlay()) {
		cerr << "Jump obstructed by piece\n";
		return false;
	}
	
	/* Move the piece */
	j.setInPlay(false);
	pieces[jumper] = &game[newx][newy];
	pieces[jumper]->setInPlay();
	pieces[jumper]->setColor(col);
	pieces[jumper]->id = jumper;
	
	p.setInPlay(false);
	other.numPieces -= 1;
	
	return true;
}
