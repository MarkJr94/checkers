#include <iostream>
#include <vector>
#include <sstream>

#include "Game.hpp"

Game::Game(const bool debug, const bool interact) :
		_WP(Bit::Masks::WP_INIT), _BP(Bit::Masks::BP_INIT), _K(0), _turn(true), _debug(
				debug), _save(), _interact(interact), _mustJump(0) {
}

Game::Game(const Save& save, const bool debug, const bool interact) :
		_debug(debug), _interact(interact) {
	restoreToSave(save);
}

Game::~Game() {
}

void Game::restoreToSave(const Save& save) {
	using namespace std;

	_save = save;
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

std::vector<Cell> Game::toArr() const {
	using Bit::Masks::S;

	std::vector<Cell> b(64);

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (_WP & S[i])
			b[i * 2 + odd] = P_W;
	}

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (_BP & S[i])
			b[i * 2 + odd] = P_B;
	}

	BitBoard WK = _WP & _K;
	if (WK) {
		for (unsigned i = 0; i < 32; i++) {
			unsigned odd = (i / 4) & 1;
			if (WK & S[i])
				b[i * 2 + odd] = K_W;
		}
	}

	BitBoard BK = _BP & _K;
	if (BK) {
		for (unsigned i = 0; i < 32; i++) {
			unsigned odd = (i / 4) & 1;
			if (BK & S[i])
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
	cout << "P1: " << Bit::bitCount(_BP) << "\tP2: " << Bit::bitCount(_WP)
			<< endl;

	std::vector<Cell> b = toArr();

	for (int j = 7; j >= 0; j--) {
		for (int i = 0; i < 8; i++)
			cout << _ctable[b[i + 8 * j]];
		cout << endl;
	}

}

BitBoard Game::getJumpers() const {
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;

	BitBoard empty = ~(_WP | _BP);
	BitBoard Temp;
	BitBoard jumpers = 0;
	if (_turn) {
		BitBoard BK = _BP & _K;
		Temp = ror(empty, 7) & _WP & CAN_UPLEFT;
		jumpers |= ror(Temp, 7) & _BP & CAN_UPLEFT;
		Temp = ror(empty, 1) & _WP & CAN_UPRIGHT;
		jumpers |= ror(Temp, 1) & _BP & CAN_UPRIGHT;

		Temp = rol(empty, 7) & _WP & CAN_DOWNRIGHT;
		jumpers |= rol(Temp, 7) & BK & CAN_DOWNRIGHT;
		Temp = rol(empty, 1) & _WP & CAN_DOWNLEFT;
		jumpers |= rol(Temp, 1) & BK & CAN_DOWNLEFT;
	} else {
		BitBoard WK = _WP & _K;
		Temp = rol(empty, 7) & _BP & CAN_DOWNRIGHT;
		jumpers |= rol(Temp, 7) & _WP & CAN_DOWNRIGHT;
		Temp = rol(empty, 1) & _BP & CAN_DOWNLEFT;
		jumpers |= rol(Temp, 1) & _WP & CAN_DOWNLEFT;

		Temp = ror(empty, 7) & _BP & CAN_UPLEFT;
		jumpers |= ror(Temp, 7) & WK & CAN_UPLEFT;
		Temp = ror(empty, 1) & _BP & CAN_UPRIGHT;
		jumpers |= ror(Temp, 1) & WK & CAN_UPRIGHT;
	}

	return jumpers;
}

BitBoard Game::getMovers() const {
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;

	const BB empty = ~(_WP | _BP);
	BB Movers;

	if (_turn) {
		const BB BK = _BP & _K;
		Movers = ror(empty, 7) & _BP & CAN_UPLEFT;
		Movers |= ror(empty, 1) & _BP & CAN_UPRIGHT;
		Movers |= rol(empty, 7) & BK & CAN_DOWNRIGHT;
		Movers |= rol(empty, 1) & BK & CAN_DOWNLEFT;
	} else {
		const BB WK = _WP & _K; // Kings
		Movers = rol(empty, 7) & _WP & CAN_DOWNRIGHT;
		Movers |= rol(empty, 1) & _WP & CAN_DOWNLEFT;
		Movers |= ror(empty, 7) & WK & CAN_UPLEFT;
		Movers |= ror(empty, 1) & WK & CAN_UPRIGHT;
	}

	return Movers;
}

/* Piece movement */
MoveCode Game::makeMove(const Move& move) {
	using namespace Bit::Masks;
	using Bit::rol;
	using Bit::ror;
	using Bit::Masks::S;

	if (move.src > 31 || move.dst > 31)
		return ILLEGAL_MOVE;

	if (_mustJump)
		return WRONG_PIECE;

	const BitBoard src = (_turn ? _BP & S[move.src] : _WP & S[move.src]);

	if (!src)
		return VOID_PIECE;

	const BitBoard empty = ~(_WP | _BP);

	BitBoard valMoves = 0;
	BB SK;
	if (_turn) {
		SK = src & _K;
		valMoves = empty & rol(src & CAN_UPLEFT, 7);
		valMoves |= empty & rol(src & CAN_UPRIGHT, 1);
		valMoves |= empty & ror(SK & CAN_DOWNRIGHT, 7);
		valMoves |= empty & ror(SK & CAN_DOWNLEFT, 1);
	} else {
		SK = src & _K;
		valMoves = empty & ror(src & CAN_DOWNRIGHT, 7);
		valMoves |= empty & ror(src & CAN_DOWNLEFT, 1);
		valMoves |= empty & rol(SK & CAN_UPLEFT, 7);
		valMoves |= empty & rol(SK & CAN_UPRIGHT, 1);
	}

	const BitBoard dst = S[move.dst];

	if (!(valMoves & dst))
		return ILLEGAL_MOVE;

	if (_turn) {
		_BP ^= src;
		_BP |= dst;
	} else {
		_WP ^= src;
		_WP |= dst;
	}

	if (SK) {
		_K ^= src;
		_K ^= dst;
	}

	if ((dst & ROW_1) || (dst & ROW_8))
		_K |= dst;

	_mustJump = 0;
	_turn = !_turn;
	return SUCCESS;
}

BB Game::canJump(const BB src, const BB vict) {
	using Bit::rol;
	using Bit::ror;
	using namespace Bit::Masks;

	if (_mustJump)
		if (src != _mustJump)
			return 0u;
	BB Temp;
	BB SK;
	BB empty = ~(_WP | _BP);

	if (_turn) {
		SK = src & _K;

		Temp = rol(src & CAN_UPLEFT, 7) & vict;
		if (Temp)
			return rol(Temp & CAN_UPLEFT, 7) & empty;
		Temp = rol(src & CAN_UPRIGHT, 1) & vict;
		if (Temp)
			return rol(Temp & CAN_UPRIGHT, 1) & empty;

		Temp = ror(SK & CAN_DOWNRIGHT, 7) & vict;
		if (Temp)
			return ror(Temp & CAN_DOWNRIGHT, 7) & empty;
		Temp = ror(SK & CAN_DOWNLEFT, 1) & vict;
		if (Temp)
			return ror(Temp & CAN_DOWNLEFT, 1) & empty;
	} else {
		SK = src & _K;

		Temp = ror(src & CAN_DOWNRIGHT, 7) & vict;
		if (Temp)
			return ror(Temp & CAN_DOWNRIGHT, 7) & empty;
		Temp = ror(src & CAN_DOWNLEFT, 1) & vict;
		if (Temp)
			return ror(Temp & CAN_DOWNLEFT, 1) & empty;

		Temp = rol(SK & CAN_UPLEFT, 7) & vict;
		if (Temp)
			return rol(Temp & CAN_UPLEFT, 7) & empty;
		Temp = rol(SK & CAN_UPRIGHT, 1) & vict;
		if (Temp)
			return rol(Temp & CAN_UPRIGHT, 1) & empty;
	}

	return 0u;
}

MoveCode Game::jump(const Move& move) {
	using namespace Bit::Masks;
	using Bit::rol;
	using Bit::ror;
	if (move.src > 31 || move.dst > 31)
		return ILLEGAL_MOVE;

	const BitBoard src = (_turn ? _BP & S[move.src] : _WP & S[move.src]);
	const BitBoard vict = (_turn ? _WP & S[move.dst] : _BP & S[move.dst]);

	if (_mustJump)
		if (src != _mustJump)
			return WRONG_PIECE;

	if (!vict || !src)
		return VOID_PIECE;

	BitBoard jumpers = getJumpers();
	if (!(jumpers & src))
		return ILLEGAL_MOVE;

	BB nextLoc = canJump(src, vict);
	if (!nextLoc)
		return ILLEGAL_MOVE;

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
	} else if ((nextLoc & ROW_8) || (nextLoc & ROW_1))
		_K ^= nextLoc;

	if (_K & vict) {
		_K ^= vict;
	}

	if (nextLoc & getJumpers()) {
		_mustJump = nextLoc;
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
