#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

std::vector<std::vector<Piece> > Player::game
									(8,std::vector<Piece> (8));

Player::Player(const Color color)
		: pieces (12), numPieces(12), col (color)
{
	int i = 0, j = 0, count = 0, id = 0;
	if (color == RED) {
		i = 7; j = 1;
		for (auto &p : pieces) {
			p.setX(j); p.setY(i);
			p.setInPlay(true);
			p.setColor(color);
			p.setIsKing(false);
			p.id = id++;
			p.addToBoard();
			j += 2;
			if (j == 8) j = 1;
			if (j == 9) j = 0;
			if ((++count == 4))  {
				--i;
				count = 0;
			}
		}
		initGame();
		return;
	}
	for (auto &p : pieces) {
		p.setX(j); p.setY(i);
		p.setInPlay(true);
		p.setColor(color);
		p.setIsKing(false);
		p.id = id++;
		p.addToBoard();
		j += 2;
		if (j == 8) j = 1;
		if (j == 9) j = 0;
		if ((++count == 4))  {
			++i;
			count = 0;
		}
	}
	initGame();
}

void Player::display() const
{
	using namespace std;
	
	cout << "Player color "  << (col == BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		p.print();
	}
}

bool Player::movePieceBlack(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != BLACK) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	pieces[piece].print();
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if (!pieces[piece].getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != pieces[piece].getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	
	unsigned nextx;
	unsigned nexty = 1 + pieces[piece].getY();
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	
	if (d == LEFT)
		nextx = pieces[piece].getX() - 1;
	else
		nextx = pieces[piece].getX() + 1;
	
	if (nextx > 7) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	if (Piece::board[nextx][nexty]) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	pieces[piece].removeFromBoard();
	pieces[piece].setX(nextx);
	pieces[piece].setY(nexty);
	pieces[piece].addToBoard();
	
	game[nextx][nexty].setColor(BLACK);
	game[nextx][nexty].id = piece;
	updateGame();
	
	return true;
}

bool Player::movePieceRed(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != RED) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	pieces[piece].print();
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if (!pieces[piece].getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != pieces[piece].getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	
	unsigned nextx;
	unsigned nexty = pieces[piece].getY() - 1;
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	
	if (d == LEFT)
		nextx = pieces[piece].getX() - 1;
	else
		nextx = pieces[piece].getX() + 1;
	
	if (nextx > 7) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	if (Piece::board[nextx][nexty]) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	pieces[piece].removeFromBoard();
	pieces[piece].setX(nextx);
	pieces[piece].setY(nexty);
	pieces[piece].addToBoard();
	
	game[nextx][nexty].setColor(BLACK);
	game[nextx][nexty].id = piece;
	updateGame();
	return true;
}

void Player::updateGame()
{
	int count;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			if (Piece::board[i][j]) {
				game[i][j].setInPlay(true);
				if (col == game[i][j].getColor()) ++count;
			} else {
				game[i][j].setInPlay(false);
			}
		}
	}
	numPieces = count;
}

void Player::printgame() const
{
	for (int j = 7; j >= 0; j--) {
		for (int i = 0; i < 8; i++) {
			if (game[i][j].getInPlay()) {
				unsigned thisid = game[i][j].id;
				std::cout << (thisid < 10 ? "0" : "")<< thisid
					<< (game[i][j].getColor() == RED? "R" : "B");
			} else {
				std::cout << "---";
			}
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Player::initGame()
{
	unsigned idno = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 8; i++) {
			Piece& alias = game[i][j];
			if (Piece::board[i][j]) {
				alias.setColor(BLACK);
				alias.id = idno++;
			} else {
				alias.setInPlay(false);
			}
		}
	}
		idno = 11;
	for (int j = 4; j < 8; j++) {
		for (int i = 7; i >= 0; i--) {
			Piece& alias = game[i][j];
			if (Piece::board[i][j]) {
				alias.setColor(RED);
				alias.id = idno--;
				alias.setInPlay(true);
			} else {
				alias.setInPlay(false);
			}
		}
	}
}

