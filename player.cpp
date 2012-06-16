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

void Player::movePieceBlack(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != BLACK) {
		cerr << "Wrong move, kid.\n";
		return;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	pieces[piece].print();
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return;
	}
	if (!pieces[piece].getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return;
	}
	if (col != pieces[piece].getColor()) {
		cerr << "Not your piece\n";
		return;
	}
	
	
	
	unsigned nexty = 1 + pieces[piece].getY();
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return;
	}
	
	if (d == LEFT) {
		unsigned leftx = pieces[piece].getX() - 1;
		if (leftx > 7) {
			cerr << "Piece obstructed at border.\n";
			return;
		}
		if (Piece::board[leftx][nexty]) {
			cerr << "Piece obstructed by piece\n";
			return;
		}
		pieces[piece].removeFromBoard();
		pieces[piece].setX(leftx);
		pieces[piece].setY(nexty);
		pieces[piece].addToBoard();
		game[leftx][nexty].id = piece;
		updateGame();
	} else {
		unsigned rightx = 1 + pieces[piece].getX();
		if (rightx > 7) {
			cerr << "Piece obstructed at border.\n";
			return;
		}
		if (Piece::board[rightx][nexty]) {
			cerr << "Piece obstructed by piece\n";
			return;
		}
		pieces[piece].removeFromBoard();
		pieces[piece].setX(rightx);
		pieces[piece].setY(nexty);
		pieces[piece].addToBoard();
		game[rightx][nexty].id = piece;
		updateGame();
	}
}

void Player::movePieceRed(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != RED) {
		cerr << "Wrong move, kid.\n";
		return;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	pieces[piece].print();
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return;
	}
	if (!pieces[piece].getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return;
	}
	if (col != pieces[piece].getColor()) {
		cerr << "Not your piece\n";
		return;
	}
	
	
	
	unsigned nexty = pieces[piece].getY() - 1;
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return;
	}
	
	if (d == LEFT) {
		unsigned leftx = pieces[piece].getX() - 1;
		if (leftx > 7) {
			cerr << "Piece obstructed at border.\n";
			return;
		}
		if (Piece::board[leftx][nexty]) {
			cerr << "Piece obstructed by piece\n";
			return;
		}
		pieces[piece].removeFromBoard();
		pieces[piece].setX(leftx);
		pieces[piece].setY(nexty);
		pieces[piece].addToBoard();
		game[leftx][nexty].id = piece;
		updateGame();
		return;
	} else {
		unsigned rightx = 1 + pieces[piece].getX();
		if (rightx > 7) {
			cerr << "Piece obstructed at border.\n";
			return;
		}
		if (Piece::board[rightx][nexty]) {
			cerr << "Piece obstructed by piece\n";
			return;
		}
		pieces[piece].removeFromBoard();
		pieces[piece].setX(rightx);
		pieces[piece].setY(nexty);
		pieces[piece].addToBoard();
		game[rightx][nexty].id = piece;
		updateGame();
	}
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
				std::cout << (thisid < 10 ? "0" : "")<< thisid;
			} else {
				std::cout << "--";
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
			if (Piece::board[i][j]) {
				game[i][j].setColor(RED);
				game[i][j].id = idno++;
			}
		}
	}
		idno = 11;
	for (int j = 4; j < 8; j++) {
		for (int i = 7; i >= 0; i--) {
			if (Piece::board[i][j]) {
				game[i][j].setColor(BLACK);
				game[i][j].id = idno--;
			}
		}
	}
}

int main()
{
	using namespace std;
	
	Player p1  (BLACK);
	p1.display();
	cout << endl <<endl;
	Player p2 (RED);
	p2.display();
	Piece::printBoard();
	p1.movePiece(9, LEFT);
	p1.movePiece(8, LEFT);
	p1.movePiece(8, RIGHT);
	p2.movePiece(10,LEFT);
	Piece::printBoard();
	p1.printgame();
	p1.display();
	return 0;
}
