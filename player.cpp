#include <iostream>

#include "checkers.hpp"
#include "player.hpp"

std::vector<std::vector<Piece> > Player::game
									(8,std::vector<Piece> (8));

typedef Piece::Color Color;

Player::Player(const Color color)
		: pieces (12, NULL), col (color), numPieces(12)
{
	for (auto &row : game) {
		for (auto &cell : row) {
			cell.setInPlay(false);
		}
	}
	
	int i = 0, j = 0, count = 0, id = 0;
	if (color == Piece::RED) {
		i = 7; j = 1;
		for (auto &q : pieces) {
			q = &game[j][i];
			Piece& p = game[j][i];
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
		return;
	}
	
	for (auto &q : pieces) {
		q = &game[j][j];
		Piece& p = game[j][i];
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
}

void Player::display() const
{
	using namespace std;
	
	cout << "Player color "  << (col == Piece::BLACK ? "Black " : "Red ");
	cout << "|| Pieces in play: " << numPieces << endl;
	for (auto &p : pieces) {
		if (p->getInPlay()) p->print();
	}
}

bool Player::movePieceBlack(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != Piece::BLACK) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	pieces[piece]->print();
	
	if (!pieces[piece]->getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != pieces[piece]->getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	
	unsigned nextx;
	unsigned nexty = 1 + pieces[piece]->getY();
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	
	if (d == LEFT)
		nextx = pieces[piece]->getX() - 1;
	else
		nextx = pieces[piece]->getX() + 1;
	
	if (nextx > 7) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	if (game[nextx][nexty].getInPlay()) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	pieces[piece]->removeFromBoard();
	pieces[piece]->setInPlay(false);
	pieces[piece] = &game[nextx][nexty];
	pieces[piece]->setInPlay(true);
	pieces[piece]->setColor(Piece::BLACK);
	pieces[piece]->addToBoard();
	
	pieces[piece]->id = piece;
	
	return true;
}

bool Player::movePieceRed(unsigned piece, Direction d)
{
	using namespace std;
	
	if (col != Piece::RED) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	pieces[piece]->print();
	
	if (!pieces[piece]->getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != pieces[piece]->getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	
	unsigned nextx;
	unsigned nexty = pieces[piece]->getY() - 1;
	if (nexty > 7 ) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	
	if (d == LEFT)
		nextx = pieces[piece]->getX() - 1;
	else
		nextx = pieces[piece]->getX() + 1;
	
	if (nextx > 7) {
		cerr << "Piece obstructed at border.\n";
		return false;
	}
	if (game[nextx][nexty].getInPlay()) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	pieces[piece]->removeFromBoard();
	pieces[piece]->setInPlay(false);
	pieces[piece] = &game[nextx][nexty];
	pieces[piece]->setInPlay(true);
	pieces[piece]->setColor(Piece::RED);
	pieces[piece]->addToBoard();
	
	pieces[piece]->id = piece;
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
					<< (game[i][j].getColor() == Piece::RED? "R" : "B");
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
	return;
}

bool Player::redJump(unsigned jumper, unsigned prey, Player& other)
{
	using namespace std;
	
	return false; /*
	if (col != Piece::RED) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	if ( jumper > 11 || prey > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	Piece& j = pieces[jumper];
	Piece& p = other.pieces[prey];
	j.print(); cout << "Preying on: "; p.print();
	
	if (!j.getInPlay() || !p.getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != j.getColor() || col == p.getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	if (p.getY() > j.getY()) {
		cerr << "Invalid target in Y direction\n";
		return false;
	}
	
	int diff = (int)p.getX() - (int)j.getX();
	cout << diff << " = diff\n";
	if (diff != 1 && diff != -1) {
		cerr << "Invalid target in X direction\n";
		return false;
	}
	
	Direction dir;
	if (diff > 0) dir = RIGHT;
	else dir = LEFT;
	unsigned newx = j.getX() + diff * 2;
	unsigned newy = j.getY() - 2;
	if (newx > 7 || newy > 7) {
		cerr << "Jump obstructed at border\n";
		return false;
	}
	
	if (Piece::board[newx][newy]) {
		cerr << "Jump obstructed by piece\n";
		return false;
	}
	
	p.removeFromBoard();
	p.setInPlay(false);
	other.numPieces -= 1;
	movePieceRed(jumper,dir);
	movePieceRed(jumper,dir);
	
	return true;*/
}

bool Player::blackJump(unsigned jumper, unsigned prey, Player& other)
{
	using namespace std;
	
	return false; /*
	if (col != Piece::BLACK) {
		cerr << "Wrong move, kid.\n";
		return false;
	}
	
	if ( jumper > 11 || prey > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	Piece& j = pieces[jumper];
	Piece& p = other.pieces[prey];
	j.print(); cout << "Preying on: "; p.print();
	
	if (!j.getInPlay() || !p.getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (col != j.getColor() || col == p.getColor()) {
		cerr << "Not your piece\n";
		return false;
	}
	
	if (p.getY() < j.getY()) {
		cerr << "Invalid target Y direction\n";
		return false;
	}

	int diff = (int)p.getX() - (int)j.getX();
	cout << diff << " = diff\n";
	if (diff != 1 && diff != -1) {
		cerr << "Invalid target X direction\n";
		return false;
	}
	
	Direction dir;
	if (diff > 0) dir = RIGHT;
	else dir = LEFT;
	unsigned newx = j.getX() + diff * 2;
	unsigned newy = j.getY() + 2;
	if (newx > 7 || newy > 7) {
		cerr << "Jump obstructed at border\n";
		return false;
	}
	
	if (Piece::board[newx][newy]) {
		cerr << "Jump obstructed by piece\n";
		return false;
	}
	
	p.removeFromBoard();
	p.setInPlay(false);
	other.numPieces -= 1;
	movePieceBlack(jumper,dir);
	movePieceBlack(jumper,dir);
	
	return true;*/
}

