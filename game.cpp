#include <iostream>
#include <vector>
#include <sstream>

#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"



Match::Match(bool db)
		: p1 (Piece::BLACK, db), p2 (Piece::RED,db),
			board (BOARD_SIZE, std::vector< Piece> (BOARD_SIZE, Piece())), turn(true),
			debug (db), save (true)
{
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].setX(i);
			board[i][j].setY(j);
			board[i][j].setInPlay(false);
		}
	}

	unsigned i = 0, j = 0, count = 0, id = 1;
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

	id = 1;
	count = 0;
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
			board (BOARD_SIZE, std::vector< Piece> (BOARD_SIZE)),
			turn(record.getTurn()), debug (db), save (record)
{
	restoreToSave(record);
}

void Match::restoreToSave(SaveGame& record)
{
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].setX(i);
			board[i][j].setY(j);
			board[i][j].setInPlay(false);
		}
	}

	turn = record.getTurn();

	vector<Piece *> *p1pieces = p1.getPieces();
	vector<Piece *> *p2pieces = p2.getPieces();

	unsigned p2numPieces = 0;
	unsigned p1numPieces = 0;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			unsigned index = record(i,j).id;
			if (record(i,j).alive) {
				board[i][j].setInPlay(true);
				board[i][j].id = index;
				if (record(i,j).color == Piece::BLACK) {
					p1numPieces++;
					board[i][j].setColor(Piece::BLACK);
					(*p1pieces)[index - 1] = &board[i][j];
				} else {
					p2numPieces++;
					board[i][j].setColor(Piece::RED);
					(*p2pieces)[index -1] = &board[i][j];
				}
			}
		}
	}

	for (auto &p : *p1pieces) {
		if (p == NULL)
			p = new Piece(~0u, 0, 0, Piece::BLACK);
	}
	for (auto &p : *p2pieces) {
		if (p == NULL)
			p = new Piece(~0u, 0, 0, Piece::RED);
	}

	p1.setnPieces(p1numPieces);
	p2.setnPieces(p2numPieces);
}

inline void Match::updateSave() {
	for (unsigned i =0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			auto & alias = board[i][j];
			save(i,j).id = alias.id;
			save(i,j).color = alias.getColor();
			save(i,j).alive = alias.getInPlay();
		}
	}
	save.setTurn(turn);
}
SaveGame Match::getSave() {
	updateSave();
	return save;
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
	if ( piece > 12 || piece < 1) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}

	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey, Player& other);
	if (turn) {
		pieces = p1.getPieces();
		alias = (*pieces)[piece - 1];
	} else {
		pieces = p2.getPieces();
		alias = (*pieces)[piece - 1];
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
	(*pieces)[piece - 1] = alias = &board[nextx][nexty];
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
		j = (*pieces)[jumper - 1];
		p = ((*p2.getPieces())[prey - 1]);
	} else {
		pieces = p2.getPieces();
		j = (*pieces)[jumper - 1];
		p = ((*p1.getPieces())[prey - 1]);
	}

	/* Testing if piece selection is valid */
	if ( jumper > 12 || prey > 12) {
		cerr << "Invalid piece number input" << endl;
		return false;
	}
	if ( jumper < 1 || prey < 1) {
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
	(*pieces)[jumper - 1] = &board[newx][newy];
	(*pieces)[jumper - 1]->setInPlay(true);
	(*pieces)[jumper - 1]->setColor(j->getColor());
	(*pieces)[jumper - 1]->id = jumper;

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
		
		if (turn) {
			if (p1.getnPieces() < 1) return;
			if (debug) cout << "P1 pieces = " << p1.getnPieces() << endl;
			while (1) {
				if (debug) print();
				if (count1 == 3) {
					count1 = 0;
					if (debug) cout << "Three failed moves P1; Lose a turn!\n\n";
					turn  =  !turn;
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
				break;
			}
		} else {
			if (p2.getnPieces() <1) return;
			if (debug) cout << "P2 pieces = " << p2.getnPieces() << endl;
			while (1) {
				if (debug) print();
				if (count2 == 3) {
					count2 = 0;
					if (debug) cout << "Three failed moves P2; Lose a turn!\n\n";
					turn = !turn;
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
				break;
			}
		}
	}
}

int Match::receiveInput(unsigned piece, Direction d)
{
	using namespace std;

	unsigned prey;
	string instring;

	cout << "Enter piece id: ";
	getline(cin,instring);
	if (instring == "q") return -1;
	if (!(stringstream(instring) >> piece)) {
		cerr << endl << piece << endl;
		cerr << "Input Error; try again\n";
		return 0;
	}
	cout << "('q' = quit)\tEnter Direction 'l' = left or 'r' = right or 'j' = jump: ";
	getline(cin,instring);
	if (instring == "q") return -1;
	/* Jumping */
	if (instring == "j") {
		if (debug) cout << "Enter prey ID: ";
		getline(cin,instring);
		if (instring == "q") return -1;
		if (!(stringstream(instring) >> prey)) {
			cerr << "Input Error; try again\n";
			return 0;
		}
		if (!jumpPiece(piece,prey)) {
			cerr << "Jumping error; try again\n";
			return 0;
		}
		return 1;
	}

	/* Regular Movement */
	if  (instring == "l")
		d = LEFT;
	else if (instring == "r")
		d = RIGHT;
	else {
		cerr << "Input Error; try again\n";
		return 0;
	}

	if (!movePiece(piece,d)) {
		cerr << "Movement error; try again\n";
		return 0;
	}
	return 1;
}

