#include <iostream>
#include <vector>
#include <sstream>

#include "Game.hpp"

Game::Game(const bool debug, const bool interact) :
		_turn(true), _debug(debug), _save(), _interact(interact), _mustJump(0) {
	using namespace std;

	_WP = Mask::WP_INIT;
	_BP = Mask::BP_INIT;
	_K = 0;

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
	const BitBoard* s = Mask::S;

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

BitBoard Game::getJumpers() const {
	using namespace Mask;
	BitBoard empty = ~(_WP | _BP);
	BitBoard Temp;
	BitBoard Movers = 0;
	if (_turn) {
		BitBoard BK = _BP & _K;
		Temp = (empty >> 4) & _WP;
		if (Temp)
			Movers |= (((Temp & DEST_R3) >> 3) | ((Temp & DEST_R5) >> 5)) & _BP;

		Temp = (((empty & DEST_R3) >> 3) | ((empty & DEST_R5) >> 5)) & _WP;
		Movers |= (Temp >> 4) & _BP;
		if (BK) {
			Temp = (empty << 4) & _WP;
			if (Temp)
				Movers |= (((Temp & DEST_L3) << 3) | ((Temp & DEST_L5) << 5))
						& BK;

			Temp = (((empty & DEST_L3) << 3) | ((empty & DEST_L5) << 5)) & _WP;
			if (Temp)
				Movers |= (Temp << 4) & BK;
		}
	} else {
		BitBoard WK = _WP & _K;
		Temp = (empty << 4) & _BP;
		if (Temp)
			Movers |= (((Temp & DEST_L3) << 3) | ((Temp & DEST_L5) << 5)) & _WP;

		Temp = (((empty & DEST_L3) << 3) | ((empty & DEST_L5) << 5)) & _BP;
		Movers |= (Temp << 4) & _WP;
		if (WK) {
			Temp = (empty >> 4) & _BP;
			if (Temp)
				Movers |= (((Temp & DEST_R3) >> 3) | ((Temp & DEST_R5) >> 5))
						& WK;

			Temp = (((empty & DEST_R3) >> 3) | ((empty & DEST_R5) >> 5)) & _BP;
			if (Temp)
				Movers |= (Temp >> 4) & WK;
		}
	}

	return Movers;
}

BitBoard Game::getMovers() const {
	using namespace Mask;

	const BB empty = ~(_WP | _BP);
	BB Movers;

	if (_turn) {
		const BB BK = _BP & _K;
		Movers = (empty >> 4) & _BP;
		Movers |= ((empty & DEST_R3) >> 3) & _BP;
		Movers |= ((empty & DEST_R5) >> 5) & _BP;
		if (BK) {
			Movers |= (empty << 4) & _BP;
			Movers |= ((empty & DEST_L3) << 4) & _BP;
			Movers |= ((empty & DEST_R3) << 4) & _BP;
		}
	} else {
		const BB WK = _WP & _K; // Kings
		Movers = (empty << 4) & _WP;
		Movers |= ((empty & DEST_L3) << 3) & _WP;
		Movers |= ((empty & DEST_L5) << 5) & _WP;
		if (WK) {
			Movers |= (empty >> 4) & WK;
			Movers |= ((empty & DEST_R3) >> 3) & WK;
			Movers |= ((empty & DEST_R5) >> 5) & WK;
		}
	}

	return Movers;
}

/* Piece movement */
MoveCode Game::makeMove(const Move& move) {
	using namespace Mask;

	if (move.src > 31 || move.dst > 31)
		return ILLEGAL;

	if (_mustJump)
		return WRONG_PIECE;

	const BitBoard src = (_turn ? _BP & S[move.src] : _WP & S[move.src]);

	if (!src)
		return ILLEGAL;

	const BitBoard empty = ~(_WP | _BP);

	BitBoard valMoves = 0;
	if (_turn) {
		valMoves = empty & (src << 4);
		valMoves |= (empty & DEST_R5) & (src << 5);
		valMoves |= (empty & DEST_R3) & (src << 3);
		if (src & _K) {
			valMoves |= empty & (src >> 4);
			valMoves |= (empty & DEST_L5) & (src >> 5);
			valMoves |= (empty & DEST_L3) & (src >> 3);
		}
	} else {
		valMoves = empty & (src >> 4);
		valMoves |= (empty & DEST_L5) & (src >> 5);
		valMoves |= (empty & DEST_L3) & (src >> 3);
		if (src & _K) {
			valMoves |= empty & (src << 4);
			valMoves |= (empty & DEST_R5) & (src << 5);
			valMoves |= (empty & DEST_R3) & (src << 3);
		}
	}

	const BitBoard dst = S[move.dst];

	if (!(valMoves & dst))
		return ILLEGAL;

	if (_turn) {
		_BP ^= src;
		_BP |= dst;
	} else {
		_WP ^= src;
		_WP |= dst;
	}

	if (src & _K) {
		_K ^= src;
		_K ^= dst;
	}

	if ((src & ROW_2) || (src & ROW_7))
		_K ^= dst;

	_mustJump = 0;
	_turn = !_turn;
	return SUCCESS;
}

MoveCode Game::jump(const Move& move) {
	using namespace Mask;
	if (move.src > 31 || move.dst > 31)
		return ILLEGAL;

	if (_mustJump)
		if (move.src != _mustJump)
			return WRONG_PIECE;

	const BitBoard src = (_turn ? _BP & S[move.src] : _WP & S[move.src]);
	const BitBoard vict = (_turn ? _WP & S[move.dst] : _BP & S[move.dst]);

	if (!vict || !src)
		return ILLEGAL;

	BitBoard jumpers = getJumpers();
	if (!(jumpers & src))
		return ILLEGAL;

	int dist = move.dst - move.src;
	unsigned abs = (
			move.dst > move.src ? move.dst - move.src : move.src - move.dst);
	if (abs != 4 && abs != 3 && abs != 5)
		return ILLEGAL;

	int next;
	if (dist < 0) {
		if (abs == 3 || abs == 5)
			next = move.dst - 4;
		else
			next = move.dst - 5;
	} else {
		if (abs == 3 || abs == 5)
			next = move.dst + 4;
		else
			next = move.dst + 3;
	}
	const BitBoard nextLoc = S[next];

	if (_turn) {
		_BP ^= src;
		_WP ^= vict;
		_BP ^= nextLoc;

	} else {
		_WP ^= src;
		_BP ^= vict;
		_WP ^= nextLoc;
	}

	if (_K & src) {
		_K ^= src;
		_K ^= nextLoc;
	}

	if (_K & vict) {
		_K ^= vict;
	}

	if ((nextLoc & ROW_8) || (nextLoc & ROW_1))
		_K ^= nextLoc;

	if (nextLoc & getJumpers()) {
		_mustJump = next;

	} else {
		_mustJump = 0;
		_turn = !_turn;
	}

	return SUCCESS;
}

MoveCode Game::receiveInput() {
	using namespace std;

	Move move;
	string instring;

	cout << "Enter 'q' at any time to quit\n";
	cout << "Enter Starting Location: ";
	getline(cin, instring);
	if (instring == "q")
		return QUIT;
	if (!(stringstream(instring) >> move.src)) {
		cerr << endl << move.src << ": ";
		cerr << "Input Error; try again\n";
		return INPUT_FAIL;
	}

	cout << "Enter Ending Location: ";
	getline(cin, instring);
	if (instring == "q")
		return QUIT;
	if (!(stringstream(instring) >> move.dst)) {
		cerr << endl << move.dst << ": ";
		cerr << "Input Error; try again\n";
		return INPUT_FAIL;
	}

	MoveCode retval;
	if ((retval = makeMove(move)) != SUCCESS) {
		cerr << "Movement Error:\n";
		cerr << _errtable[retval];
	}
	return retval;
}
