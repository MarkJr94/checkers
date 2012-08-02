#include <iostream>
#include <vector>
#include <sstream>

#include "checkers.hpp"
//#include "player.hpp"
#include "game.hpp"

SaveGame::SaveGame(bool turn) :
		data(8, std::vector<cellRecord>(8)), turn(turn) {
}

SaveGame& SaveGame::operator=(const SaveGame other) {
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			data[i][j] = other.data[i][j];
		}
	}
	this->turn = other.turn;
	this->mustJump = other.mustJump;
	return *this;
}

void SaveGame::write(std::string fname) {
	using namespace std;

	fstream savefile;
	savefile.open(fname.c_str(), fstream::trunc | fstream::out);

	savefile << turn << " " << mustJump << endl;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile << data[i][j].id << " " << data[i][j].alive << " "
					<< data[i][j].color << " " << data[i][j].isKing << " ";
		}
		savefile << endl;
	}
	savefile.close();
}
void SaveGame::read(std::string fname) {
	using namespace std;

	fstream savefile;
	savefile.open(fname.c_str(), fstream::in);
	bool turn, alive, isKing;
	unsigned col, id, mj;

	savefile >> turn >> mj;
	this->turn = turn;
	this->mustJump = mj;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			savefile >> id >> alive >> col >> isKing;
			data[i][j].color = (Piece::Color) col;
			data[i][j].alive = alive;
			data[i][j].id = id;
			data[i][j].isKing = isKing;
		}
	}
	savefile.close();
}

Game::Game(bool db, bool interact) :
		board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE, Piece())), turn(true), debug(
				db), save(true), interact(interact), mustJump(0) {
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].x = i;
			board[i][j].y = j;
			board[i][j].inPlay = false;
		}
	}

	unsigned i = 0, j = 0, count = 0, id = 1;

	for (unsigned k = 1; k <= 12; k++) {
		p1[k] = &board[j][i];
		p1[k]->x = j;
		p1[k]->y = i;
		p1[k]->inPlay = true;
		p1[k]->col = Piece::BLACK;
		p1[k]->isKing = false;
		p1[k]->id = id++;
		j += 2;
		if (j == BOARD_SIZE)
			j = 1;
		if (j == BOARD_SIZE + 1)
			j = 0;
		if (++count == 4) {
			++i;
			count = 0;
		}
		p1[k]->print();
	}

	id = 1;
	count = 0;
	i = 7, j = 1;

	for (unsigned k = 1; k <= 12; k++) {
		p2[k] = &board[j][i];
		p2[k]->x = j;
		p2[k]->y = i;
		p2[k]->inPlay = true;
		p2[k]->col = Piece::RED;
		p2[k]->isKing = false;
		p2[k]->id = id++;
		j += 2;
		if (j == BOARD_SIZE)
			j = 1;
		if (j == BOARD_SIZE + 1)
			j = 0;
		if (++count == 4) {
			--i;
			count = 0;
		}
		p2[k]->print();
	}
}

Game::Game(SaveGame record, bool db, bool interact) :
		board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE)), turn(
				record.getTurn()), debug(db), save(record), interact(interact) {
	restoreToSave(record);
}

void Game::restoreToSave(SaveGame& record) {
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			board[i][j].x = i;
			board[i][j].y = j;
			board[i][j].inPlay = false;
		}
	}

	turn = record.getTurn();
	mustJump = record.getMustJump();

	unsigned p2numPieces = 0;
	unsigned p1numPieces = 0;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			unsigned index = record(i, j).id;
			if (record[i][j].alive) {
				board[i][j].inPlay = (true);
				board[i][j].id = (index);
				if (record[i][j].isKing)
					board[i][j].isKing = (true);
				if (record[i][j].color == Piece::BLACK) {
					p1numPieces++;
					board[i][j].col = (Piece::BLACK);
					p1[index] = &board[i][j];
				} else {
					p2numPieces++;
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
			save[i][j].id = alias.id;
			save[i][j].color = alias.col;
			save[i][j].alive = alias.inPlay;
			save[i][j].isKing = alias.isKing;
		}
	}
	save.setTurn(turn);
	save.setMustJump(mustJump);
}
SaveGame Game::getSave() {
	updateSave();
	return save;
}

void Game::print() const {
	_print();
}

void Game::_print() const {
	using namespace std;

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

	if (mustJump) {
		if (interact)
			cerr << "movePiece: You must jump" << endl;
		return false;
	}

	/* Testing if piece selection is valid */
	if (piece > 12 || piece < 1) {
		if (interact)
			cerr << "movePiece: Invalid piece number input" << endl;
		return false;
	}

	/* Jumping */
	map<int, Piece *>& pieces = (turn ? p1 : p2);

	if (pieces.find(piece) == pieces.end()) {
		if (interact)
			cerr << "movePiece: Selected piece " << piece << " not in play\n";
		return false;
	}

	alias = pieces[piece];

	/* Directions BKLEFT and BKRIGHT are only valid for kings */
	bool wasKing = alias->isKing;
	if (!wasKing) {
		if (d == BKLEFT || d == BKRIGHT) {
			if (interact)
				cerr << "movePiece: Invalid direction for non-King piece.\n";
			return false;
		}
	}

	if (debug)
		alias->print();

	/* Determine next coordinates for jump */
	unsigned nextx, nexty;

	if (turn) {
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
	if (nexty > 7) {
		if (interact)
			cerr << "movePiece: Piece obstructed at border.\n";
		return false;
	}

	if (nextx > 7) {
		if (interact)
			cerr << "movePiece: Piece obstructed at border.\n";
		return false;
	}
	if (board[nextx][nexty].inPlay) {
		if (interact)
			cerr << "movePiece: Piece obstructed by piece\n";
		return false;
	}

	/* Complete the move */
	alias->inPlay = false;
	pieces[piece] = alias = &board[nextx][nexty];
	alias->inPlay = true;
	if (turn)
		alias->col = Piece::BLACK;
	else
		alias->col = Piece::RED;
	alias->id = piece;
	if (wasKing || nexty == 7 || nexty == 0)
		alias->isKing = true;

	turn = !turn;
	return true;
}

/* Jumping */
bool Game::jumpPiece(unsigned jumper, unsigned prey) {
	using namespace std;

	Piece *j, *p;
	if (mustJump) {
		if (jumper != mustJump) {
			if (interact)
				cerr << "movePiece: You must continue your jump"
						<< " with the same piece." << endl;
			return false;
		}
	}

	map<int, Piece *>& pieces = (turn ? p1 : p2);
	map<int, Piece *>& other = (turn ? p2 : p1);

	/* Testing if piece selection is valid */
	if (jumper > 12 || prey > 12) {
		if (interact)
			cerr << "pieceJump: Invalid piece number input" << endl;
		return false;
	}
	if (jumper < 1 || prey < 1) {
		if (interact)
			cerr << "pieceJump: Invalid piece number input" << endl;
		return false;
	}

	if (pieces.find(jumper) == pieces.end()
			|| other.find(prey) == other.end()) {
		if (interact)
			cerr << "pieceJump: Selected piece not inplay\n";
		return false;
	}

	j = pieces[jumper];
	p = other[prey];

	if (!j->inPlay || !p->inPlay) {
		if (interact)
			cerr << "pieceJump: Selected piece not inplay\n";
		return false;
	}
	if (debug) {
		j->print();
		cout << "Preying on: ";
		p->print();
	}

	/* Testing if valid targets */
	bool wasKing = j->isKing;
	if (!wasKing) {
		if (!turn) {
			if (p->y > j->y) {
				if (interact)
					cerr << "pieceJump: Invalid target in Y direction\n";
				return false;
			}
		} else {
			if (p->y < j->y) {
				if (interact)
					cerr << "pieceJump: Invalid target in Y direction\n";
				return false;
			}
		}
	}

	int ydiff = (int) p->y - (int) j->y;
	if (ydiff != 1 && ydiff != -1) {
		if (interact)
			cerr << "pieceJump: Invalid target in Y direction\n";
		return false;
	}
	int diff = (int) p->x - (int) j->x;
	if (diff != 1 && diff != -1) {
		if (interact)
			cerr << "pieceJump: Invalid target in X direction\n";
		return false;
	}

	/* Testing the validity of the jump */
	unsigned newx = j->x + diff * 2;
	unsigned newy = j->y + ydiff * 2;

	if (newx > 7 || newy > 7) {
		if (interact)
			cerr << "pieceJump: Jump obstructed at border\n";
		return false;
	}
	if (board[newx][newy].inPlay) {
		if (interact)
			cerr << "pieceJump: Jump obstructed by piece\n";
		return false;
	}

	/* Move the piece */
	j->inPlay = false;
	pieces[jumper] = &board[newx][newy];
	pieces[jumper]->inPlay = true;
	pieces[jumper]->col = j->col;
	pieces[jumper]->id = jumper;
	if (wasKing)
		pieces[jumper]->isKing = true;

	p->inPlay = false;
	other.erase(prey);

	turn = !turn;
	setMustJump(jumper);
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
		if (debug)
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
//		setMustJump(piece);
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

