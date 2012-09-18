#include <iostream>
#include <vector>
#include <sstream>

#include "Game.hpp"

Game::Game(const bool debug, const bool interact) :
		mWP(Bit::Masks::WP_INIT), mBP(Bit::Masks::BP_INIT), mK(0), mTurn(true), mDebug(
				debug), mSave(), mInteract(interact), mMustJump(0) {
}

Game::Game(const Save& save, const bool debug, const bool interact) :
		mDebug(debug), mInteract(interact) {
	restoreToSave(save);
}

Game::~Game() {
}

void Game::restoreToSave(const Save& save) {
	using namespace std;

	mSave = save;
	mWP = save.WP;
	mBP = save.BP;
	mK = save.K;
	mTurn = save.turn;
	mMustJump = save.mustJump;
}

inline void Game::updateSave() {
	mSave.WP = mWP;
	mSave.BP = mBP;
	mSave.K = mK;
	mSave.turn = mTurn;
	mSave.mustJump = mMustJump;
}

Save Game::getSave() {
	updateSave();
	return mSave;
}

std::vector<Cell> Game::toArr() const {
	using Bit::Masks::S;

	std::vector<Cell> b(64);

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (mWP & S[i])
			b[i * 2 + odd] = P_W;
	}

	for (unsigned i = 0; i < 32; i++) {
		unsigned odd = (i / 4) & 1;
		if (mBP & S[i])
			b[i * 2 + odd] = P_B;
	}

	BitBoard WK = mWP & mK;
	if (WK) {
		for (unsigned i = 0; i < 32; i++) {
			unsigned odd = (i / 4) & 1;
			if (WK & S[i])
				b[i * 2 + odd] = K_W;
		}
	}

	BitBoard BK = mBP & mK;
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

	if (mTurn)
		cout << "Player 1's turn.\n";
	else
		cout << "Player 2's turn.\n";
	cout << "P1: " << Bit::bitCount(mBP) << "\tP2: " << Bit::bitCount(mWP)
			<< endl;

	std::vector<Cell> b = toArr();

	for (int j = 7; j >= 0; j--) {
		for (int i = 0; i < 8; i++)
			cout << cellTable[b[i + 8 * j]];
		cout << endl;
	}

}

BitBoard Game::getJumpers() const {
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;

	BitBoard empty = ~(mWP | mBP);
	BitBoard Temp;
	BitBoard jumpers = 0;
	if (mTurn) {
		BitBoard BK = mBP & mK;
		Temp = ror(empty, 7) & mWP & CAN_UPLEFT;
		jumpers |= ror(Temp, 7) & mBP & CAN_UPLEFT;
		Temp = ror(empty, 1) & mWP & CAN_UPRIGHT;
		jumpers |= ror(Temp, 1) & mBP & CAN_UPRIGHT;

		Temp = rol(empty, 7) & mWP & CAN_DOWNRIGHT;
		jumpers |= rol(Temp, 7) & BK & CAN_DOWNRIGHT;
		Temp = rol(empty, 1) & mWP & CAN_DOWNLEFT;
		jumpers |= rol(Temp, 1) & BK & CAN_DOWNLEFT;
	} else {
		BitBoard WK = mWP & mK;
		Temp = rol(empty, 7) & mBP & CAN_DOWNRIGHT;
		jumpers |= rol(Temp, 7) & mWP & CAN_DOWNRIGHT;
		Temp = rol(empty, 1) & mBP & CAN_DOWNLEFT;
		jumpers |= rol(Temp, 1) & mWP & CAN_DOWNLEFT;

		Temp = ror(empty, 7) & mBP & CAN_UPLEFT;
		jumpers |= ror(Temp, 7) & WK & CAN_UPLEFT;
		Temp = ror(empty, 1) & mBP & CAN_UPRIGHT;
		jumpers |= ror(Temp, 1) & WK & CAN_UPRIGHT;
	}

	return jumpers;
}

BitBoard Game::getMovers() const {
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;

	const BB empty = ~(mWP | mBP);
	BB Movers;

	if (mTurn) {
		const BB BK = mBP & mK;
		Movers = ror(empty, 7) & mBP & CAN_UPLEFT;
		Movers |= ror(empty, 1) & mBP & CAN_UPRIGHT;
		Movers |= rol(empty, 7) & BK & CAN_DOWNRIGHT;
		Movers |= rol(empty, 1) & BK & CAN_DOWNLEFT;
	} else {
		const BB WK = mWP & mK; // Kings
		Movers = rol(empty, 7) & mWP & CAN_DOWNRIGHT;
		Movers |= rol(empty, 1) & mWP & CAN_DOWNLEFT;
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

	if (mMustJump)
		return WRONG_PIECE;

	const BitBoard src = (mTurn ? mBP & S[move.src] : mWP & S[move.src]);

	if (!src)
		return VOID_PIECE;

	const BitBoard empty = ~(mWP | mBP);

	BitBoard valMoves = 0;
	BB SK;
	if (mTurn) {
		SK = src & mK;
		valMoves = empty & rol(src & CAN_UPLEFT, 7);
		valMoves |= empty & rol(src & CAN_UPRIGHT, 1);
		valMoves |= empty & ror(SK & CAN_DOWNRIGHT, 7);
		valMoves |= empty & ror(SK & CAN_DOWNLEFT, 1);
	} else {
		SK = src & mK;
		valMoves = empty & ror(src & CAN_DOWNRIGHT, 7);
		valMoves |= empty & ror(src & CAN_DOWNLEFT, 1);
		valMoves |= empty & rol(SK & CAN_UPLEFT, 7);
		valMoves |= empty & rol(SK & CAN_UPRIGHT, 1);
	}

	const BitBoard dst = S[move.dst];

	if (!(valMoves & dst))
		return ILLEGAL_MOVE;

	if (mTurn) {
		mBP ^= src;
		mBP |= dst;
	} else {
		mWP ^= src;
		mWP |= dst;
	}

	if (SK) {
		mK ^= src;
		mK ^= dst;
	}

	if ((dst & ROW_1) || (dst & ROW_8))
		mK |= dst;

	mMustJump = 0;
	mTurn = !mTurn;
	return SUCCESS;
}

BB Game::canJump(const BB src, const BB vict) {
	using Bit::rol;
	using Bit::ror;
	using namespace Bit::Masks;

	if (mMustJump)
		if (src != mMustJump)
			return 0u;
	BB Temp;
	BB SK;
	BB empty = ~(mWP | mBP);

	if (mTurn) {
		SK = src & mK;

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
		SK = src & mK;

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

	const BitBoard src = (mTurn ? mBP & S[move.src] : mWP & S[move.src]);
	const BitBoard vict = (mTurn ? mWP & S[move.dst] : mBP & S[move.dst]);

	if (mMustJump)
		if (src != mMustJump)
			return WRONG_PIECE;

	if (!vict || !src)
		return VOID_PIECE;

	BitBoard jumpers = getJumpers();
	if (!(jumpers & src))
		return ILLEGAL_MOVE;

	BB nextLoc = canJump(src, vict);
	if (!nextLoc)
		return ILLEGAL_MOVE;

	if (mTurn) {
		mBP ^= src;
		mWP ^= vict;
		mBP ^= nextLoc;

	} else {
		mWP ^= src;
		mBP ^= vict;
		mWP ^= nextLoc;
	}

	if (mK & src) {
		mK ^= src;
		mK ^= nextLoc;
	} else if ((nextLoc & ROW_8) || (nextLoc & ROW_1))
		mK ^= nextLoc;

	if (mK & vict) {
		mK ^= vict;
	}

	if (nextLoc & getJumpers()) {
		mMustJump = nextLoc;
	} else {
		mMustJump = 0;
		mTurn = !mTurn;
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
		cerr << errorTable[retval];
	}
	return retval;
}
