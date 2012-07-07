#include <iostream>
#include <vector>

#include "checkers.hpp"
#include "game.hpp"
#include "game.hpp"



Match::Match(bool db)
		: p1 (Piece::BLACK, db), p2 (Piece::RED,db), 
			board (BOARD_SIZE, std::vector< Piece> (BOARD_SIZE, Piece())), turn(true),
			debug (db)
{
	using namespace std;
	
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].setX(i);
			board[i][j].setY(j);
			board[i][j].setInPlay(false);
		}
	}
	
	unsigned i = 0, j = 0, count = 0, id = 0;
	vector<Piece *> * pieces = (p1.getPieces());
	
	for (auto &p : *pieces) {
		p = &board[j][i];
		p->setX(j); p->setY(i);
		p->setInPlay(true);
		p->setColor(Piece::BLACK);
		p->setIsKing(false);
		p->id = id++;
		j += 2;
		if (j == BOARD_SIZE) j = 1;
		if (j == BOARD_SIZE + 1) j = 0;
		if ((++count == 4))  {
			++i;
			count = 0;
		}
	}
	
	id = count = 0;
	i = 7, j = 1;
	
	pieces = (p2.getPieces());
	for (auto &p : *pieces) {
		p = &board[j][i];
		p->setX(j); p->setY(i);
		p->setInPlay(true);
		p->setColor(Piece::RED);
		p->setIsKing(false);
		p->id = id++;
		j += 2;
		if (j == BOARD_SIZE) j = 1;
		if (j == BOARD_SIZE + 1) j = 0;
		if ((++count == 4))  {
			--i;
			count = 0;
		}
	}
}

Match::Match(SaveGame record, bool db)
		: p1 (Piece::BLACK, db), p2 (Piece::RED,db), 
			board (BOARD_SIZE, std::vector< Piece> (BOARD_SIZE)), turn(true),
			debug (db)
{
	using namespace std;
	
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].setX(i);
			board[i][j].setY(j);
		}
	}
	
	vector<Piece *> p1pieces = *p1.getPieces();
	vector<Piece *> p2pieces = *p2.getPieces();
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			if (record(i,j).alive) {
				board[i][j].setInPlay(true);
				board[i][j].id = record(i,j).id;
				if (record(i,j).color == Piece::BLACK) {
					board[i][j].setColor(Piece::BLACK);
					p1pieces[board[i][j].id] = &board[i][j];
				} else {
					board[i][j].setColor(Piece::RED);
					p1pieces[board[i][j].id] = &board[i][j];
				}
			}
		}
	}
}

void Match::print() const
{
	using namespace std;
	
	for (int j = (int)(BOARD_SIZE - 1); j >= 0; j--) {
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			if (board[i][j].getInPlay()) {
				unsigned thisid = board[i][j].id;
				cout << (thisid < 10 ? "0" : "")<< thisid
					<< (board[i][j].getColor() == Piece::RED? "R" : "B");
			} else {
				cout << "---";
			}
		}
		cout << "\n";
	}
	cout << endl;
}

/* Piece movement */
bool Match::movePiece(unsigned piece, Direction d)
{
	using namespace std;
	Piece * alias;
	vector<Piece *> * pieces;
	
	/* Testing if piece selection is valid */
	if ( piece > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey, Player& other);
	if (turn) {
		pieces = p1.getPieces();
		alias = (*pieces)[piece];
	} else {
		pieces = p2.getPieces();
		alias = (*pieces)[piece];
	}
	
	alias->getIsKing();
	
	
	if (!alias->getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	
	if (debug) alias->print();
	if (debug) cout << "Piece No. " << piece << " Direction " 
		<< (d == LEFT ? "LEFT" : "RIGHT") << endl;
	
	/* Determine next coordinates for jump */
	unsigned nextx, nexty;
	
	if (turn)
		nexty = alias->getY() + 1;
	else
		nexty = alias->getY() - 1;
		
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
	if (board[nextx][nexty].getInPlay()) {
		cerr << "Piece obstructed by piece\n";
		return false;
	}
	
	/* Complete the move */
	alias->setInPlay(false);
	(*pieces)[piece] = alias = &board[nextx][nexty];
	alias->setInPlay(true);
	if (turn)
		alias->setColor(Piece::BLACK);
	else
		alias->setColor(Piece::RED);
	alias->id = piece;
	
	turn = !turn;
	return true;
}

/* Jumping */
bool Match::jumpPiece(unsigned jumper, unsigned prey)
{
	using namespace std;
	Piece *j, *p;
	vector<Piece *> * pieces;
	if (turn) {
		pieces = p1.getPieces();
		j = (*pieces)[jumper];
		p = ((*p2.getPieces())[prey]);
	} else {
		pieces = p2.getPieces();
		j = (*pieces)[jumper];
		p = ((*p1.getPieces())[prey]);
	}
	
	/* Testing if piece selection is valid */
	if ( jumper > 11 || prey > 11) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if (!j->getInPlay() || !p->getInPlay() ) {
		cerr << "Selected piece not inplay\n";
		return false;
	}
	if (debug) {j->print(); cout << "Preying on: "; p->print();}
	
	/* Testing if valid targets */
	if (!turn) {
		if (p->getY() > j->getY()) {
			cerr << "Invalid target in Y direction\n";
			return false;
		}
	} else {
		if (p->getY() < j->getY()) {
			cerr << "Invalid target in Y direction\n";
			return false;
		}
	}
	int ydiff = (int)p->getY() - (int)j->getY();
	if (ydiff != 1 && ydiff != -1) {
		cerr << "Invalid target in Y direction\n";
		return false;
	}
	int diff = (int)p->getX() - (int)j->getX();
	if (diff != 1 && diff != -1) {
		cerr << "Invalid target in X direction\n";
		return false;
	}
	
	/* Testing the validity of the jump */
	unsigned newx = j->getX() + diff * 2;
	unsigned newy;
	if (!turn) {
		newy = j->getY() - 2;
	} else {
		newy = j->getY() + 2;
	}
	if (newx > 7 || newy > 7) {
		cerr << "Jump obstructed at border\n";
		return false;
	}
	if (board[newx][newy].getInPlay()) {
		cerr << "Jump obstructed by piece\n";
		return false;
	}
	
	/* Move the piece */
	j->setInPlay(false);
	(*pieces)[jumper] = &board[newx][newy];
	(*pieces)[jumper]->setInPlay(true);
	(*pieces)[jumper]->setColor(j->getColor());
	(*pieces)[jumper]->id = jumper;
	
	p->setInPlay(false);
	if (turn)
		p2.setnPieces(p2.getnPieces() -1 );
	else
		p1.setnPieces(p1.getnPieces() -1 );
	
	turn = !turn;
	return true;
}

void Match::play()
{
	using namespace std;
	
	if (debug) {
		p1.display();
		p2.display();
		print();
	}
	
	unsigned count1 = 0,count2 = 0;
	unsigned piece;
	Direction d;
	
	while (1) {
		if (debug) cout << p1.getnPieces() << " Player 1\n" << p2.getnPieces() << " Player 2\n\n";
		if (p1.getnPieces() < 1) return;
		if (debug) cout << "P1 pieces = " << p1.getnPieces() << endl;
		while (1) {
			if (count1 == 3) {
				count1 = 0;
				if (debug) cout << "Three failed moves P1; Lose a turn!\n\n";
				break;
			}
			if (debug) cout << "================ Player 1 (Black) ==============\n\n";
			int c = receiveInput(piece,d);
			switch (c) {
				case -1: return;
				case 0: {
					count1++;
					continue;
				}
				default:;
			}
			count1 = 0;
			if (debug) print();
			break;
		}
		if (p2.getnPieces() <1) return;
		if (debug) cout << "P2 pieces = " << p2.getnPieces() << endl;
		while (1) {
			if (count2 == 3) {
				count2 = 0;
				if (debug) cout << "Three failed moves P2; Lose a turn!\n\n";
				break;
			}
			if (debug) cout << "================ Player 2  (Red) ==============\n\n";
			int c = receiveInput(piece,d);
			switch (c) {
				case -1: return;
				case 0: {
					count2++;
					continue;
				}
				default:;
			}
			count2 = 0;
			if (debug) print();
			break;
		}
	}
}

int Match::receiveInput(unsigned piece, Direction d)
{
	using namespace std;
	string dirString;
	unsigned prey;
	Player *a, *b;
	if (turn) {
		a = &p1;
		b = &p2;
	} else {
		a = &p2;
		b = &p1;
	}
	Player& player = *a;
	Player& other = *b;
	
	if (debug) cout << "Enter piece id: ";
	if (!(cin  >> piece)) {
		cin.clear();
		cerr << "Input Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		return 0;
	}
	if (debug) cout << "('q' = quit)\tEnter Direction 'l' = left or 'r' = right or 'j' = jump: ";
	cin >> dirString;
	if (dirString == "q") return -1;
	/* Jumping */
	if (dirString == "j") {
		if (debug) cout << "Enter prey ID: ";
		cin >> prey;
		if (!jumpPiece(piece,prey)) {
			cerr << "Jumping error; try again\n";
			return 0;
		}
		return 1;
	}
	
	/* Regular Movement */
	if  (dirString == "l")
		d = LEFT;
	else if (dirString == "r")
		d = RIGHT;
	else {
		cerr << "Input Error\n";
		return 0;
	}
	
	if (!movePiece(piece,d)) {
		cerr << "Movement error; try again\n";
		return 0;
	}
	return 1;
}

int main()
{
	using namespace std;
	
	Match test;
	test.play();
	return 0;
}
