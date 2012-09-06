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

Game* AI::_game = new Game(false, false);

AI::AI(const unsigned degree, const Save& save, const Move& creator,
		const unsigned difficulty) :
		_level(degree), _difficulty(difficulty), _children(), _move(creator), _save(
				save), _p1Avg(0), _p2Avg(0) {
	srand(time(NULL));

}

AI::~AI() {
	for (auto child : _children)
		delete child;

}

void AI::resetToSave(const Save& save) {
	_save = save;
	_p1Avg = _game->getP1score();
	_p2Avg = _game->getP2score();

	for (auto child : _children)
		delete child;

	_children.clear();
}

void AI::printScene() {
	using namespace std;

	cout << "This is a degree " << _level << " AI with " << "P1 Min: " << _p1Avg
			<< "\tP2 Min: " << _p2Avg << endl;
//	Save oldsave = scenario->getSave();
//	scenario->restoreToSave(_save);
//	scenario->print();
//	scenario->restoreToSave(oldsave);
}

//void AI::recursivePrint() {
//	printScene();
//	if (children.empty())
//		return;
//
//	for (auto& child : children) {
//		child->recursivePrint();
//	}
//}

unsigned AI::testMoves() {
	using namespace std;

	bool retval;
	unsigned successCount = 0;

	_game->restoreToSave(_save);

	MoveRecord origin;
	unsigned jumpCount = 0;
	for (unsigned piece = 1; piece <= 12; piece++) {
		/* Test jumping */
		for (unsigned prey = 1; prey <= 12; prey++) {
			_game->restoreToSave(_save);
			retval = _game->jumpPiece(piece, prey);
			if (retval) {
				++jumpCount;
				++successCount;
				origin.jump = true;
				origin.piece = piece;
				origin.prey = prey;
				if (canMultiJump(*_game)) {
					_game->_turn = !_game->_turn;
				} else {
				_game->setMustJump(0);
				}
				_children.push_back(
						new AI(_level + 1, _game->getSave(), origin, _difficulty));
			}
		}
	}
	if (jumpCount > 0)
		return successCount;

	origin.jump = false;
	for (unsigned piece = 1; piece <= 12; piece++) {

		/* Test left move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, LEFT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = LEFT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin, _difficulty));
		}

		/* Test right move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, RIGHT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = RIGHT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin, _difficulty));
		}

		/* Test back right move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, BKRIGHT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = BKRIGHT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin, _difficulty));
		}

		/* Test back left move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, BKLEFT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = BKLEFT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin, _difficulty));
		}

	}
	return successCount;
}

unsigned AI::generateOutcomes() {

	if (_level == _difficulty -1)
		return 0;

	unsigned num = testMoves();
	if (num < 1)
		return 0;

	for (auto child : _children)
		child->generateOutcomes();

	return num;
}

void AI::updateScores() {
	if (_children.empty())
		return;

	for (auto& child : _children)
		child->updateScores();

	float temp1 = 0, temp2 = 0;
	for (auto& child : _children) {
		temp1 += child->_p1Avg;
		temp2 += child->_p2Avg;
	}

	_p1Avg = temp1 / _children.size();
	_p2Avg = temp2 / _children.size();
}

Move AI::evaluateMoves(bool optimizeForP1, bool aggro) {

	unsigned nKids = generateOutcomes();
	updateScores();

	if (nKids == 0) {
		std::cerr << "NO MOVES.\n";
		_move = {0, 0};
		return _move;
	}

//	aggro = rand();

	unsigned favoredSon = 0;
	float bestAvg;
	if (aggro) {
		bestAvg = 999999999;
		if (optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				if (_children[i]->_p2Avg < bestAvg) {
					bestAvg = _children[i]->_p2Avg;
					favoredSon = i;

				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				if (_children[i]->_p1Avg < bestAvg) {
					bestAvg = _children[i]->_p1Avg;
					favoredSon = i;
				}
			}
		}

	} else {
		bestAvg = 0.0;
		if (optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				if (_children[i]->_p1Avg > bestAvg) {
					bestAvg = _children[i]->_p1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				if (_children[i]->_p2Avg > bestAvg) {
					bestAvg = _children[i]->_p2Avg;
					favoredSon = i;
				}
			}
		}
	}

	return _children[favoredSon]->_move;
}

Move AI::evaluateGame(Game& game) {
	resetToSave(game.getSave());

	return evaluateMoves(game.getTurn(), true);
}

Move AI::getRandomMove() const {
	unsigned index = rand() % _children.size();

	return _children[index]->_move;
}

bool AI::canMultiJump(const Game& game) {
	_game->restoreToSave(game._save);
	_game->_turn = !_game->_turn;
	const Save savestate = _game->getSave();

	unsigned piece = _game->_mustJump;
	/* Test jumping */
	bool retval;
	for (unsigned prey = 1; prey <= 12; prey++) {
		retval = _game->jumpPiece(piece, prey);
		if (retval) {
			_game->restoreToSave(savestate);
			return true;
		}
	}

//	_game->restoreToSave(savestate);
	return false;
}

