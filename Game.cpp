#include <iostream>
#include <vector>
#include <sstream>

#include "Piece.hpp"
#include "Game.hpp"

Save Game::templateSave;

Game::Game(const bool debug, const bool interact) :
		_board(new Cell*[BOARD_SIZE]), _turn(true), _debug(debug), _save(), _interact(
				interact), _mustJump(0), _p1Score(12), _p2Score(12) {
	using namespace std;

	for (int i = 0; i < BOARD_SIZE; i++)
		_board[i] = new Cell[BOARD_SIZE];

	restoreToSave(templateSave);
}

Game::Game(const Save& save, const bool debug, const bool interact) :
		_board(new Cell*[BOARD_SIZE]), _debug(debug), _save(save), _interact(
				interact) {
	for (int i = 0; i < BOARD_SIZE; i++)
		_board[i] = new Cell[10];

	restoreToSave(save);
}

Game::~Game() {
	for (int i = 0; i < BOARD_SIZE; i++)
		delete[] _board[i];
	delete[] _board;
}

void Game::restoreToSave(const Save& save) {
	using namespace std;

	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			_board[i][j] = save[i][j];
		}
	}

	_turn = save.turn;
	_mustJump = save.mustJump;
}

inline void Game::updateSave() {
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			_save[i][j] = _board[i][j];
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
	using std::cout;
	using std::endl;

	if (_turn)
		cout << "Player 1's turn.\n";
	else
		cout << "Player 2's turn.\n";
	cout << "P1: " << (int)_p1Score << "\tP2: " << (int)_p2Score << endl;
	for (int j = (int) (BOARD_SIZE - 1); j >= 0; j--) {
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			cout << _pptable[_board[i][j]];
		}
		cout << "\n";
	}
	cout << endl;
}

/* Piece movement */

MoveCode Game::makeMove(const Move& move) {
	Coord src = move.src;
	Coord dst = move.dst;

	if (src.x > 7 || src.y > 7)
		return ILLEGAL;

	if (_board[src.x][src.y] == EMPTY)
		return WRONG_PIECE;

	if (dst.x > 7 || dst.y > 7 || _board[dst.x][dst.y] != EMPTY)
		return OBSTRUCTED;

	switch (_board[src.x][src.y]) {
	case P_BLACK:
		return moveBlack(move);
	case P_RED:
		return moveRed(move);
	case K_BLACK:
		return moveKBlack(move);
	case K_RED:
		return moveKRed(move);
	default:
		return ILLEGAL;
	}
}

MoveCode Game::moveBlack(const Move& move) {
	if (!_turn) return WRONG_PIECE;

	Coord dst = move.dst;
	Coord src = move.src;

	int xdiff = dst.x - src.x;
	if (xdiff == 1 || xdiff == -1) {

		int ydiff = dst.y - src.y;
		if (ydiff != 1)
			return ILLEGAL;

		_board[dst.x][dst.y] = P_BLACK;
		_board[src.x][src.y] = EMPTY;

		_turn = !_turn;
		return SUCCESS;
	} else  if (xdiff == 2 || xdiff == -2) {
		int ydiff = dst.y - src.y;
		if (ydiff != 2)
			return ILLEGAL;

		Coord victim { src.x + xdiff /2,src.y + ydiff/2};

		if (_board[victim.x][victim.y] != P_RED && _board[victim.x][victim.y] != K_RED) return ILLEGAL;

		_board[dst.x][dst.y] = P_BLACK;
		_board[victim.x][victim.y] = EMPTY;
		_board[src.x][src.y] = EMPTY;

		--_p2Score;
		_turn = !_turn;
		return SUCCESS;

	} else {
		return ILLEGAL;
	}
}

MoveCode Game::moveRed(const Move& move) {
	if (_turn) return WRONG_PIECE;

	Coord dst = move.dst;
	Coord src = move.src;

	int xdiff = dst.x - src.x;
	if (xdiff == 1 || xdiff == -1) {

		int ydiff = dst.y - src.y;
		if (ydiff != -1)
			return ILLEGAL;

		_board[dst.x][dst.y] = P_RED;
		_board[src.x][src.y] = EMPTY;

		_turn = !_turn;
		return SUCCESS;
	} else  if (xdiff == 2 || xdiff == -2) {
		int ydiff = dst.y - src.y;
		if (ydiff != -2)
			return ILLEGAL;

		Coord victim { src.x + xdiff /2,src.y + ydiff/2};

		if (_board[victim.x][victim.y] != P_BLACK && _board[victim.x][victim.y] != K_BLACK) return ILLEGAL;

		_board[dst.x][dst.y] = P_RED;
		_board[victim.x][victim.y] = EMPTY;
		_board[src.x][src.y] = EMPTY;

		--_p1Score;
		_turn = !_turn;
		return SUCCESS;

	} else {
		return ILLEGAL;
	}
}

MoveCode Game::moveKBlack(const Move& move) {
	if (!_turn) return WRONG_PIECE;

	Coord dst = move.dst;
	Coord src = move.src;

	int xdiff = dst.x - src.x;
	if (xdiff == 1 || xdiff == -1) {

		int ydiff = dst.y - src.y;
		if (ydiff != 1 && ydiff != -1)
			return ILLEGAL;

		_board[dst.x][dst.y] = K_BLACK;
		_board[src.x][src.y] = EMPTY;

		_turn = !_turn;
		return SUCCESS;
	} else  if (xdiff == 2 || xdiff == -2) {
		int ydiff = dst.y - src.y;
		if (ydiff != 2 && ydiff != -2)
			return ILLEGAL;

		Coord victim { src.x + xdiff /2,src.y + ydiff/2};

		if (_board[victim.x][victim.y] != P_RED && _board[victim.x][victim.y] != K_RED) return ILLEGAL;

		_board[dst.x][dst.y] = K_BLACK;
		_board[victim.x][victim.y] = EMPTY;
		_board[src.x][src.y] = EMPTY;

		--_p2Score;
		_turn = !_turn;
		return SUCCESS;

	} else {
		return ILLEGAL;
	}
}

MoveCode Game::moveKRed(const Move& move) {
	if (_turn) return WRONG_PIECE;

	Coord dst = move.dst;
	Coord src = move.src;

	int xdiff = dst.x - src.x;
	if (xdiff == 1 || xdiff == -1) {

		int ydiff = dst.y - src.y;
		if (ydiff != 1 && ydiff != -1)
			return ILLEGAL;

		_board[dst.x][dst.y] = K_RED;
		_board[src.x][src.y] = EMPTY;

		_turn = !_turn;
		return SUCCESS;
	} else  if (xdiff == 2 || xdiff == -2) {
		int ydiff = dst.y - src.y;
		if (ydiff != 2 && ydiff != -2)
			return ILLEGAL;

		Coord victim { src.x + xdiff /2,src.y + ydiff/2};

		if (_board[victim.x][victim.y] != P_BLACK && _board[victim.x][victim.y] != K_BLACK) return ILLEGAL;

		_board[dst.x][dst.y] = K_RED;
		_board[victim.x][victim.y] = EMPTY;
		_board[src.x][src.y] = EMPTY;

		--_p1Score;
		_turn = !_turn;
		return SUCCESS;

	} else {
		return ILLEGAL;
	}
}


//Hash::Zkey Game::getHash() const {
//	using Hash::ZobristTable;
//	using Hash::Zkey;
//
//	Zkey hash = 0;
//	const ZobristTable& zt = ZobristTable::instance();
//
//	for (auto& pair : _p1) {
//		const Piece* p = pair.second;
//		hash ^= zt[p->isKing][Piece::BLACK][p->x + 8 * p->y];
//	}
//	for (auto& pair : _p2) {
//		const Piece* p = pair.second;
//		hash ^= zt[p->isKing][Piece::RED][p->x + 8 * p->y];
//	}
//
//	return hash;
//}
//
int Game::receiveInput() {
	using namespace std;

	Coord src;
	string instring;

	cout << "Enter 'q' at any time to quit\n";
	cout << "Enter Starting Coordinates: ";
	getline(cin, instring);
	if (instring == "q")
		return -1;
	if (!(stringstream(instring) >> src.x >> src.y)) {
		cerr << endl << src.x << src.y;
		cerr << "Input Error; try again\n";
		return 0;
	}

	Coord dst;
	cout << "Enter Ending Coordinates: ";
	getline(cin, instring);
	if (instring == "q")
		return -1;
	if (!(stringstream(instring) >> dst.x >> dst.y)) {
		cerr << endl << dst.x << dst.y;
		cerr << "Input Error; try again\n";
		return 0;
	}


	MoveCode retval;
	if ((retval = makeMove({src,dst})) != SUCCESS) {
		cerr << "Movement Error:\n";
		cerr << _errtable[retval];
	}
	return 1;
}
