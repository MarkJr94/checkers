#include <iostream>
#include <vector>
#include <sstream>

#include "Piece.hpp"
#include "Game.hpp"

Save Game::templateSave;

Game::Game(const bool debug, const bool interact) :
		board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece())), _turn(true), _debug(
				debug), _save(true), _interact(interact), _mustJump(0) {
	using namespace std;

	restoreToSave(templateSave);
}

Game::Game(const Save& save, const bool debug, const bool interact) :
		board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE)), _debug(debug), _save(
				save), _interact(interact) {
	restoreToSave(save);
}

Game::~Game() {

}

void Game::restoreToSave(const Save& record) {
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].x = i;
			board[i][j].y = j;
			board[i][j].inPlay = false;
		}
	}

	_turn = record.turn;
	_mustJump = record.mustJump;

//	unsigned p2numPieces = 0;
//	unsigned p1numPieces = 0;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			unsigned index = record[i][j].id;
			if (record[i][j].alive) {
				board[i][j].inPlay = (true);
				board[i][j].id = index;
				if (record[i][j].isKing)
					board[i][j].isKing = (true);
				if (record[i][j].color == Piece::BLACK) {
//					p1numPieces++;
					board[i][j].col = (Piece::BLACK);
					p1[index] = &board[i][j];
				} else {
//					p2numPieces++;
					board[i][j].col = (Piece::RED);
					p2[index] = &board[i][j];
				}
			}
		}
	}
}

inline void Game::updateSave() {
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			auto & alias = board[i][j];
			_save[i][j].id = alias.id;
			_save[i][j].color = alias.col;
			_save[i][j].alive = alias.inPlay;
			_save[i][j].isKing = alias.isKing;
		}
	}
	_save.turn = _turn;
	_save.mustJump = _mustJump;
}

Save Game::getSave() {
	updateSave();
	return _save;
}

void Game::print() {
	using namespace std;

	if (_turn)
		cout << "Player 1's turn.\n";
	else
		cout << "Player 2's turn.\n";
	cout << "P1: " << p1.size() << "\tP2: " << p2.size() << endl;
	for (int j = (int) (BOARD_SIZE - 1); j >= 0; j--) {
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			if (board[i][j].inPlay) {
				unsigned thisid = board[i][j].id;
				cout << (thisid < 10 ? "0" : "") << thisid;
				if (board[i][j].isKing) {
					cout << (board[i][j].col == Piece::RED ? "R" : "B") << "K";
				} else {
					cout << (board[i][j].col == Piece::RED ? "RE" : "BL");
				}
			} else {
				cout << "----";
			}
		}
		cout << "\n";
	}
	cout << endl;
}

/* Piece movement */
bool Game::movePiece(unsigned piece, Direction d) {
	using namespace std;

	Piece * alias;

//	if (mustJump) {
//		if (interact)
//			cerr << "movePiece: You must jump" << endl;
//		return false;
//	}

	/* Testing if piece selection is valid */
	if (piece > 12 || piece < 1) {
		if (_interact)
			cerr << "movePiece: Invalid piece number input" << endl;
		return false;
	}

	/* Jumping */
	map<int, Piece *>& pieces = (_turn ? p1 : p2);

	if (pieces.find(piece) == pieces.end()) {
		if (_interact)
			cerr << "movePiece: Selected piece " << piece << " not in play\n";
		return false;
	}

	alias = pieces[piece];

	/* Directions BKLEFT and BKRIGHT are only valid for kings */
	bool wasKing = alias->isKing;
	if (!wasKing) {
		if (d == BKLEFT || d == BKRIGHT) {
			if (_interact)
				cerr << "movePiece: Invalid direction for non-King piece.\n";
			return false;
		}
	}

	if (_debug)
		alias->print(cout);

	/* Determine next coordinates for jump */
	unsigned nextx, nexty;

	if (_turn) {
		if (d == BKLEFT || d == BKRIGHT)
			nexty = alias->y - 1;
		else
			nexty = alias->y + 1;
	} else {
		if (d == BKLEFT || d == BKRIGHT)
			nexty = alias->y + 1;
		else
			nexty = alias->y - 1;
	}

	if (d == LEFT || d == BKLEFT)
		nextx = alias->x - 1;
	else
		nextx = alias->x + 1;

	/* Testing move validity */
	if (nexty > 7 || nextx > 7) {
		if (_interact)
			cerr << "movePiece: Piece obstructed at border.\n";
		return false;
	}

	if (board[nextx][nexty].inPlay) {
		if (_interact)
			cerr << "movePiece: Piece obstructed by piece\n";
		return false;
	}

	/* Complete the move */
	alias->inPlay = false;
	pieces[piece] = alias = &board[nextx][nexty];

	alias->inPlay = true;
	if (_turn)
		alias->col = Piece::BLACK;
	else
		alias->col = Piece::RED;
	alias->id = piece;
	if (wasKing || nexty == 7 || nexty == 0)
		alias->isKing = true;

	_turn = !_turn;
	return true;
}

/* Jumping */
bool Game::jumpPiece(unsigned jumper, unsigned prey) {
	using namespace std;

	Piece *j, *p;
//	if (mustJump) {
//		if (jumper != mustJump) {
//			if (interact)
//				cerr << "movePiece: You must continue your jump"
//						<< " with the same piece." << endl;
//			return false;
//		}
//	}

	map<int, Piece *>& pieces = (_turn ? p1 : p2);
	map<int, Piece *>& other = (_turn ? p2 : p1);

	/* Testing if piece selection is valid */
	if (jumper > 12 || prey > 12) {
		if (_interact)
			cerr << "pieceJump: Invalid piece number input" << endl;
		return false;
	}
	if (jumper < 1 || prey < 1) {
		if (_interact)
			cerr << "pieceJump: Invalid piece number input" << endl;
		return false;
	}

	if (pieces.find(jumper) == pieces.end()
			|| other.find(prey) == other.end()) {
		if (_interact)
			cerr << "pieceJump: Selected piece not on board\n";
		return false;
	}

	j = pieces[jumper];
	p = other[prey];

	if (!j->inPlay || !p->inPlay) {
		if (_interact)
			cerr << "pieceJump: Selected piece not in play\n";
		return false;
	}
	if (_debug) {
		j->print(cout);
		cout << "Preying on: ";
		p->print(cout);
	}

	/* Testing if valid targets */
	bool wasKing = j->isKing;
	if (!wasKing) {
		if (!_turn) {
			if (p->y > j->y) {
				if (_interact)
					cerr << "pieceJump: Invalid target in Y direction\n";
				return false;
			}
		} else {
			if (p->y < j->y) {
				if (_interact)
					cerr << "pieceJump: Invalid target in Y direction\n";
				return false;
			}
		}
	}

	int ydiff = (int) p->y - (int) j->y;
	if (ydiff != 1 && ydiff != -1) {
		if (_interact)
			cerr << "pieceJump: Target too far in Y direction\n";
		return false;
	}
	int xdiff = (int) p->x - (int) j->x;
	if (xdiff != 1 && xdiff != -1) {
		if (_interact)
			cerr << "pieceJump: Target too far in X direction\n";
		return false;
	}

	/* Testing the validity of the jump */
	unsigned newx = j->x + xdiff * 2;
	unsigned newy = j->y + ydiff * 2;

	if (newx > 7 || newy > 7) {
		if (_interact)
			cerr << "pieceJump: Jump obstructed at border\n";
		return false;
	}
	if (board[newx][newy].inPlay) {
		if (_interact)
			cerr << "pieceJump: Jump obstructed by piece\n";
		return false;
	}

	/* Move the piece */

	j->inPlay = false;
	Piece::Color oldCol = j->col;

	pieces[jumper] = j = &board[newx][newy];
	j->inPlay = true;
	j->col = oldCol;
	j->id = jumper;
	if (wasKing)
		pieces[jumper]->isKing = true;

	p->inPlay = false;
	other.erase(prey);

	_turn = !_turn;
	_mustJump = jumper;
	return true;
}

int Game::receiveInput() {
	using namespace std;

	unsigned piece, prey;
	Direction d;
	string instring;

	cout << "Enter piece id: ";
	getline(cin, instring);
	if (instring == "q")
		return -1;
	if (!(stringstream(instring) >> piece)) {
		cerr << endl << piece << endl;
		cerr << "Input Error; try again\n";
		return 0;
	}
	cout << "('q' = quit)\tEnter Direction 'l' = left or 'r' = right or "
			<< "'bl' = back left or 'br' = back right or 'j' = jump: ";
	getline(cin, instring);
	if (instring == "q")
		return -1;
	/* Jumping */
	if (instring == "j") {
		if (_debug)
			cout << "Enter prey ID: ";
		getline(cin, instring);
		if (instring == "q")
			return -1;
		if (!(stringstream(instring) >> prey)) {
			cerr << "Input Error; try again\n";
			return 0;
		}
		if (!jumpPiece(piece, prey)) {
			cerr << "Jumping error; try again\n";
			return 0;
		}
		return 2;
	}

	/* Regular Movement */
	if (instring == "l")
		d = LEFT;
	else if (instring == "r")
		d = RIGHT;
	else if (instring == "bl")
		d = BKLEFT;
	else if (instring == "br")
		d = BKRIGHT;
	else {
		cout << "Input Error; try again\n";
		return 0;
	}

	if (!movePiece(piece, d)) {
		cerr << "Movement error; try again\n";
		return 0;
	}
	return 1;
}
