#include <iostream>
#include <vector>
#include <sstream>

#include "Game.hpp"

Save Game::_templateSave;

const Masks& Game::_masks = Masks::inst();

Game::Game(const bool debug, const bool interact) :
		_turn(true), _debug(debug), _save(), _interact(interact), _mustJump(0) {
	using namespace std;

	_WP = _masks.WP_INIT;
	_BP = _masks.BP_INIT;
	_K = 0;

	restoreToSave(_templateSave);
}

Game::Game(const Save& save, const bool debug, const bool interact) :
		_debug(debug), _save(save), _interact(interact) {
	restoreToSave(save);
}

Game::~Game() {
}

void Game::restoreToSave(const Save& save) {
	using namespace std;

	_WP = save.WP;
	_BP = save.BP;
	_K = save.K;
	_turn = save.turn;
	_mustJump = save.mustJump;
}

inline void Game::updateSave() {
	_save.WP = _WP;
	_save.BP = _BP;
	_save.K = _K;
	_save.turn = _turn;
	_save.mustJump = _mustJump;
}

Save Game::getSave() {
	updateSave();
	return _save;
}

Cell* Game::toArr() const {
	const BitBoard* s = _masks.S;

	Cell* b = new Cell[64]();

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (_WP & s[i])
			b[i * 2 + odd] = P_W;
	}

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (_BP & s[i])
			b[i * 2 + odd] = P_B;
	}

	BitBoard WK = _WP & _K;
	if (WK) {
		for (unsigned i = 0; i < 32; i++) {
			unsigned odd = (i / 4) & 1;
			if (WK & s[i])
				b[i * 2 + odd] = K_W;
		}
	}

	BitBoard BK = _BP & _K;
	if (BK) {
		for (unsigned i = 0; i < 32; i++) {
			unsigned odd = (i / 4) & 1;
			if (WK & s[i])
				b[i * 2 + odd] = K_B;
		}
	}

	return b;
}

void Game::print() const {
	using std::cout;
	using std::endl;

	if (_turn)
		cout << "Player 1's turn.\n";
	else
		cout << "Player 2's turn.\n";
	cout << "P1: " << getP1score() << "\tP2: " << getP2score() << endl;

	Cell* b = toArr();

	for (int j = 7; j >= 0; j--) {
		for (int i = 0; i < 8; i++)
			cout << _ctable[b[i + 8 * j]];
		cout << endl;
	}

	delete[] b;
}

/* Piece movement */

MoveCode Game::makeMove(const Move& move) {
	if (move.src > 32 || move.dst > 32)
		return ILLEGAL;

	BitBoard src;
	if (_turn)
		src = _BP & _masks.S[move.src];
	else
		src = _WP & _masks.S[move.src];

	if (!src)
		return ILLEGAL;

	BitBoard valMoves = 0;
	BitBoard dst = _masks.S[move.dst];

	BitBoard empty = ~(_WP | _BP);
	if (_turn) {
		valMoves = empty & (src << 4);
		valMoves |= (empty & _masks.DEST_R5) & (src << 5);
		valMoves |= (empty & _masks.DEST_R3) & (src << 3);
		if (src & _K) {
			valMoves |= empty & (src >> 4);
			valMoves |= (empty & _masks.DEST_L5) & (src >> 5);
			valMoves |= (empty & _masks.DEST_L3) & (src >> 3);
		}
	} else {
		valMoves = empty & (src >> 4);
		valMoves |= (empty & _masks.DEST_L5) & (src >> 5);
		valMoves |= (empty & _masks.DEST_L3) & (src >> 3);
		if (src & _K) {
			valMoves |= empty & (src << 4);
			valMoves |= (empty & _masks.DEST_R5) & (src << 5);
			valMoves |= (empty & _masks.DEST_R3) & (src << 3);
		}
	}

	if (!(valMoves & dst))
		return ILLEGAL;

	if (_turn) {
		_BP ^= src;
		_BP |= dst;
	} else {
		_WP ^= src;
		_WP |= dst;
		_K ^= src;
		_K |= dst;
	}

	_turn = !_turn;
	return SUCCESS;
}


//
//inline bool onBoard(const Coord& co) {
//	if (co.x > 7 || co.x > 7)
//		return false;
//	return true;
//}

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
//int Game::receiveInput() {
//	using namespace std;
//
//	Coord src;
//	string instring;
//
//	cout << "Enter 'q' at any time to quit\n";
//	cout << "Enter Starting Coordinates: ";
//	getline(cin, instring);
//	if (instring == "q")
//		return -1;
//	if (!(stringstream(instring) >> src.x >> src.y)) {
//		cerr << endl << src.x << src.y;
//		cerr << "Input Error; try again\n";
//		return 0;
//	}
//
//	Coord dst;
//	cout << "Enter Ending Coordinates: ";
//	getline(cin, instring);
//	if (instring == "q")
//		return -1;
//	if (!(stringstream(instring) >> dst.x >> dst.y)) {
//		cerr << endl << dst.x << dst.y;
//		cerr << "Input Error; try again\n";
//		return 0;
//	}
//
//	MoveCode retval;
//	if ((retval = makeMove( { src, dst })) != SUCCESS) {
//		cerr << "Movement Error:\n";
//		cerr << _errtable[retval];
//	}
//	return 1;
//}
