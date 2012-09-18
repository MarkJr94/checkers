/*
 * AI.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "AI.hpp"

Game* AI::sGame = new Game(false, false);

//const bool db = true;
const bool db = false;
const bool dbg = true;

void printMove(const Move& move)
{
	std::cout << "SRC: " << move.src << " DST: " << move.dst << "\t";
}

AI::AI(const unsigned degree, const Save& save, const unsigned difficulty) :
				mLevel(degree),
				mDifficulty(difficulty),
				mChildren(),
				mMoves(),
				mSave(save),
				mP1Avg(0),
				mP2Avg(0)
{
	srand(time(NULL));
}

AI::~AI()
{
	for (auto child : mChildren)
		delete child;
}

void AI::initialize(const Save& save)
{
	mSave = save;
	sGame->restoreToSave(save);
//	_p1Avg = _game->getP1score();
//	_p2Avg = _game->getP2score();
	mP1Avg = sGame->p1NumPieces();
	mP2Avg = sGame->p2NumPieces();
	for (auto child : mChildren)
		delete child;
	mChildren.clear();

	mMoves.clear();
}

void AI::printScene()
{
	using namespace std;

	cout << "This is a degree " << mLevel << " AI with " << "P1 Min: " << mP1Avg
			<< "\tP2 Min: " << mP2Avg << endl;
}

void AI::generateMovesBlack()
{
//	using namespace std;
	using namespace Bit::Masks;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB Movers = sGame->getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = sGame->getEmpty();
	BB target;
	while (Movers) {
		const BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;
		if ((target = ((rol(mover & CAN_UPLEFT, 7) & empty))))
			mMoves.push_back( { bbUMap[mover], bbUMap[target] });
		if ((target = ((rol(mover & CAN_UPRIGHT, 1) & empty))))
			mMoves.push_back( { bbUMap[mover], bbUMap[target] });

		if (mover & sGame->mK) {
			if ((target = ((ror(mover & CAN_DOWNRIGHT, 7) & empty))))
				mMoves.push_back( { bbUMap[mover], bbUMap[target] });
			if ((target = ((ror(mover & CAN_DOWNLEFT, 1) & empty))))
				mMoves.push_back( { bbUMap[mover], bbUMap[target] });
		}
	}
}

void AI::generateMovesWhite()
{
	using namespace std;
	using namespace Bit::Masks;
	using Bit::ror;
	using Bit::rol;
	using Bit::highBit;

	BB Movers = sGame->getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = sGame->getEmpty();
	BB target;
	while (Movers) {
		BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;

		if ((target = ((ror(mover & CAN_DOWNRIGHT, 7) & empty))))
			mMoves.push_back( { bbUMap[mover], bbUMap[target] });
		if ((target = ((ror(mover & CAN_DOWNLEFT, 1) & empty))))
			mMoves.push_back( { bbUMap[mover], bbUMap[target] });

		if (mover & sGame->mK) {
			if ((target = ((rol(mover & CAN_UPLEFT, 7) & empty))))
				mMoves.push_back( { bbUMap[mover], bbUMap[target] });
			if ((target = ((rol(mover & CAN_UPRIGHT, 1) & empty))))
				mMoves.push_back( { bbUMap[mover], bbUMap[target] });
		}
	}
}

void AI::generateJumpsBlack()
{
	using namespace std;
	using Bit::Masks::bbUMap;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB jumpers = sGame->getJumpers();
//	_game->print();
//	std::cout << "++++++++++++" << hex << jumpers << dec << std::endl;

	while (jumpers) {
		BB j = highBit(jumpers);
//		std::cout << "++++++++++++jumper = :" << hex << j << dec <<std::endl;
		jumpers ^= j;
		BB victims = sGame->mWP;
		BB vict;
		vict = rol(j, 7) & victims;
		if (sGame->canJump(j, vict)) {
			mMoves.push_back( { bbUMap[j], bbUMap[vict] });
		}
		vict = rol(j, 1) & victims;
		if (sGame->canJump(j, vict)) {
			mMoves.push_back( { bbUMap[j], bbUMap[vict] });
		}

		if (j & sGame->mK) {
			vict = ror(j, 7) & victims;
			if (sGame->canJump(j, vict)) {
				mMoves.push_back( { bbUMap[j], bbUMap[vict] });
			}
			vict = ror(j, 1) & victims;
			if (sGame->canJump(j, vict)) {
				mMoves.push_back( { bbUMap[j], bbUMap[vict] });
			}
		}
	}
}

void AI::generateJumpsWhite()
{
	using namespace std;
	using Bit::Masks::bbUMap;
	using Bit::rol;
	using Bit::ror;
	using Bit::highBit;

	BB jumpers = sGame->getJumpers();
//	std::cout << "++++++++++++" << hex << jumpers << dec <<std::endl;

	while (jumpers) {
		BB j = highBit(jumpers);
//		std::cout << "++++++++++++jumper = :" << hex << j << dec <<std::endl;
		jumpers ^= j;
		BB victims = sGame->mBP;

		BB vict = ror(j, 7) & victims;
		if (sGame->canJump(j, vict)) {
			mMoves.push_back( { bbUMap[j], bbUMap[vict] });
		}
		vict = ror(j, 1) & victims;
		if (sGame->canJump(j, vict)) {
			mMoves.push_back( { bbUMap[j], bbUMap[vict] });
		}

		if (j & sGame->mK) {
			vict = rol(j, 7) & victims;
			if (sGame->canJump(j, vict)) {
				mMoves.push_back( { bbUMap[j], bbUMap[vict] });
			}
			vict = rol(j, 1) & victims;
			if (sGame->canJump(j, vict)) {
				mMoves.push_back( { bbUMap[j], bbUMap[vict] });
			}
		}
	}
}

std::pair<bool, unsigned> AI::generateOutcomes()
{
	using std::cout;
	using std::endl;

	initialize(mSave);

//	printScene();

	if (mLevel == mDifficulty)
		return {0,0};

	bool jumps = sGame->getJumpers();
	if (jumps) {
		generateJumps();
	} else {
		generateMoves();
	}

	unsigned numOutcomes = mMoves.size();
//	if (dbg && _level == 0) std::cout << "Number of OutComes: " << numOutcomes << std::endl;
	if (numOutcomes < 1)
		return {0,0};

	if (jumps) {
		for (unsigned i = 0; i < numOutcomes; i++) {
			if (db) {
				printMove(mMoves[i]);
				cout << "Turn: " << sGame->mTurn << " " << "Jump: "
						<< errorTable[sGame->jump(mMoves[i])] << endl;
			} else
				sGame->jump(mMoves[i]);
			mChildren.push_back(
					new AI(mLevel + 1, sGame->getSave(), mDifficulty));
			sGame->restoreToSave(mSave);
		}
	} else {
		for (unsigned i = 0; i < numOutcomes; i++) {
			if (db) {
				printMove(mMoves[i]);
				cout << "Turn: " << sGame->mTurn << " " << "Move: "
						<< errorTable[sGame->makeMove(mMoves[i])] << endl;
			} else
				sGame->makeMove(mMoves[i]);
			mChildren.push_back(
					new AI(mLevel + 1, sGame->getSave(), mDifficulty));
			sGame->restoreToSave(mSave);
		}
	}

	for (auto child : mChildren)
		child->generateOutcomes();

	return {jumps, numOutcomes};
}

void AI::updateScores()
{
	if (mChildren.empty())
		return;

	for (auto& child : mChildren)
		child->updateScores();

	float temp1 = 0, temp2 = 0;
	for (auto& child : mChildren) {
		temp1 += child->mP1Avg;
		temp2 += child->mP2Avg;
	}

	mP1Avg = temp1 / mChildren.size();
	mP2Avg = temp2 / mChildren.size();
}

std::pair<Move, bool> AI::evaluateMoves(bool aggro)
{

	bool optimizeForP1 = sGame->mTurn;
	std::pair<bool, unsigned> info = generateOutcomes();
	updateScores();

	unsigned nKids = info.second;

	if (nKids == 0) {
		std::cerr << "NO MOVES.\n";
		return { {0, 0},0};
	}

	unsigned favoredSon = 0;
	float bestAvg;
	if (aggro) {
		bestAvg = 999999999;
		if (optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				if (mChildren[i]->mP2Avg < bestAvg) {
					bestAvg = mChildren[i]->mP2Avg;
					favoredSon = i;

				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				if (mChildren[i]->mP1Avg < bestAvg) {
					bestAvg = mChildren[i]->mP1Avg;
					favoredSon = i;
				}
			}
		}

	} else {
		bestAvg = 0.0;
		if (optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				if (mChildren[i]->mP1Avg > bestAvg) {
					bestAvg = mChildren[i]->mP1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				if (mChildren[i]->mP2Avg > bestAvg) {
					bestAvg = mChildren[i]->mP2Avg;
					favoredSon = i;
				}
			}
		}
	}

	return {mMoves[favoredSon],info.first};
}

std::pair<Move, bool> AI::evaluateGame(Game& game)
{
	mSave = game.getSave();

	return evaluateMoves(false);
}

Move AI::getRandomMove() const
{
	unsigned index = rand() % mChildren.size();

	return mMoves[index];
}

