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

void printMove(const Move& move) {
	std::cout << "SRC: " << move.src << " DST: " << move.dst << std::endl;
}

AI::AI(const unsigned degree, const Save& save, const unsigned difficulty) :
		_level(degree), _difficulty(difficulty), _children(), _moves(), _save(
				save), _p1Avg(0), _p2Avg(0) {
	srand(time(NULL));

}

AI::~AI() {
	for (auto child : _children)
		delete child;
}

void AI::initialize(const Save& save) {
	_save = save;
	_p1Avg = _game->getP1score();
	_p2Avg = _game->getP2score();
	_game->restoreToSave(save);

	for (auto child : _children)
		delete child;
	_children.clear();

	_moves.clear();
}

void AI::printScene() {
	using namespace std;

	cout << "This is a degree " << _level << " AI with " << "P1 Min: " << _p1Avg
			<< "\tP2 Min: " << _p2Avg << endl;
}

void AI::generateMovesBlack() {
	using namespace std;
	using Mask::bbUMap;
	using Mask::ROW_EVEN;
	using Mask::ROW_ODD;
	using Mask::highBit;

	BB Movers = _game->getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = _game->getEmpty();
	BB target;
	while (Movers) {
		BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;
		if ((target = ((mover << 4) & empty)))
			_moves.push_back( { bbUMap[mover], bbUMap[target] });
		if (mover & ROW_ODD) {
			if ((target = ((mover << 5) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
		} else {
			if ((target = ((mover << 3) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
		}

		if (mover & _game->_K) {
			if ((target = ((mover >> 4) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
			if (mover & ROW_ODD) {
				if ((target = ((mover >> 3) & empty)))
					_moves.push_back( { bbUMap[mover], bbUMap[target] });
			} else {
				if ((target = ((mover >> 5) & empty)))
					_moves.push_back( { bbUMap[mover], bbUMap[target] });
			}
		}
	}
}

void AI::generateMovesWhite() {
	using namespace std;
	using Mask::bbUMap;
	using Mask::ROW_EVEN;
	using Mask::ROW_ODD;
	using Mask::highBit;

	BB Movers = _game->getMovers();
//	std::cout << "++++++++++++" << hex << Movers << dec <<std::endl;

	BB empty = _game->getEmpty();
	BB target;
	while (Movers) {
		BB mover = highBit(Movers);
//		std::cout << "++++++++++++mover = :" << hex << mover << dec <<std::endl;
		Movers ^= mover;

		if ((target = ((mover >> 4) & empty)))
			_moves.push_back( { bbUMap[mover], bbUMap[target] });
		if (mover & ROW_ODD) {
			if ((target = ((mover >> 3) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
		} else {
			if ((target = ((mover >> 5) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
		}

		if (mover & _game->_K) {
			if ((target = ((mover << 4) & empty)))
				_moves.push_back( { bbUMap[mover], bbUMap[target] });
			if (mover & ROW_ODD) {
				if ((target = ((mover << 5) & empty)))
					_moves.push_back( { bbUMap[mover], bbUMap[target] });
			} else {
				if ((target = ((mover << 3) & empty)))
					_moves.push_back( { bbUMap[mover], bbUMap[target] });
			}
		}
	}
}

void AI::generateJumpsBlack() {
	using namespace std;
	using Mask::bbUMap;
	using Mask::ROW_EVEN;
	using Mask::ROW_ODD;
	using Mask::highBit;

	BB jumpers = _game->getJumpers();
//	std::cout << "++++++++++++" << hex << jumpers << dec << std::endl;

	BB empty = _game->getEmpty();
	while (jumpers) {
		BB j = highBit(jumpers);
		jumpers ^= j;
//		std::cout << "++++++++++++jumper = :" << hex << j << dec <<std::endl;
		BB victims = _game->_WP;
		BB vict;
		bool odd = j & ROW_ODD;
		if (odd) {
			vict = (j << 4) & victims;
			if ((vict << 3) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });

			vict = (j << 5) & victims;
			if ((vict << 4) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });
		} else {
			vict = (j << 4) & victims;
			if ((vict << 5) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });

			vict = (j << 3) & victims;
			if ((vict << 4) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });
		}

		if (j & _game->_K) {
			if (odd) {
				vict = (j >> 4) & victims;
				if ((vict >> 5) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });

				vict = (j >> 3) & victims;
				if ((vict >> 4) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });
			} else {
				vict = (j >> 4) & victims;
				if ((vict >> 3) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });

				vict = (j >> 5) & victims;
				if ((vict >> 4) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });
			}
		}
	}
}

void AI::generateJumpsWhite() {
	using namespace std;
	using Mask::bbUMap;
	using Mask::ROW_EVEN;
	using Mask::ROW_ODD;
	using Mask::highBit;

	BB jumpers = _game->getJumpers();
//	std::cout << "++++++++++++" << hex << jumpers << dec <<std::endl;

	BB empty = _game->getEmpty();
	while (jumpers) {
		BB j = highBit(jumpers);
		jumpers ^= j;
//		std::cout << "++++++++++++jumper = :" << hex << j << dec <<std::endl;
		BB victims = _game->_BP;
		BB vict;
		bool odd = j & ROW_ODD;
		if (odd) {
			vict = (j >> 4) & victims;
			if ((vict >> 5) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });

			vict = (j >> 3) & victims;
			if ((vict >> 4) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });
		} else {
			vict = (j >> 4) & victims;
			if ((vict >> 3) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });

			vict = (j >> 5) & victims;
			if ((vict >> 4) & empty)
				_moves.push_back( { bbUMap[j], bbUMap[vict] });
		}

		if (j & _game->_K) {
			if (odd) {
				vict = (j << 4) & victims;
				if ((vict << 3) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });

				vict = (j << 5) & victims;
				if ((vict << 4) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });
			} else {
				vict = (j << 4) & victims;
				if ((vict << 5) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });

				vict = (j << 3) & victims;
				if ((vict << 4) & empty)
					_moves.push_back( { bbUMap[j], bbUMap[vict] });
			}
		}
	}
}
std::pair<bool,unsigned> AI::generateOutcomes() {
	using std::cout; using std::endl;

	if (_level == _difficulty - 1)
		return {0,0};

	bool jumps = true;
	generateJumps();
	if (_moves.empty()) {
		generateMoves();
		jumps = false;
	}
	unsigned numOutcomes = _moves.size();
//	std::cout << "Number of OutComes: " << numOutcomes << std::endl;
	if (numOutcomes < 1)
		return {0,0};

	if (jumps) {
//		_children.resize(numOutcomes);
		for (unsigned i = 0; i < numOutcomes; i++) {
//			printMove(_moves[i]);
			cout << _game->_turn << " " << "Jump: " << _errtable[_game->jump(_moves[i])] << endl;
//			_game->jump(_moves[i]);
			_children.push_back(new AI(_level + 1,_game->getSave(),_difficulty));
			_game->restoreToSave(_save);
		}
	} else {
		for (unsigned i = 0; i < numOutcomes; i++) {
//			printMove(_moves[i]);
			cout << _game->_turn << " "  << "Move: " << _errtable[_game->makeMove(_moves[i])] << endl;
//			_game->makeMove(_moves[i]);
			_children.push_back(new AI(_level + 1,_game->getSave(),_difficulty));
			_game->restoreToSave(_save);
		}
	}

	for (auto child : _children)
		child->generateOutcomes();

	return {jumps, numOutcomes};
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

	std::pair<bool,unsigned> info = generateOutcomes();
	updateScores();

	unsigned nKids = info.second;

	if (nKids == 0) {
		std::cerr << "NO MOVES.\n";
		return {0, 0};
	}

	aggro = rand();

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

	return _moves[favoredSon];
}

std::pair<Move,bool> AI::evaluateGame(Game& game) {
	initialize(_game->getSave());
	bool optimizeForP1 = _game->_turn;

	std::pair<bool,unsigned> info = generateOutcomes();
	updateScores();

	unsigned nKids = info.second;

	if (nKids == 0) {
		std::cerr << "NO MOVES.\n";
		return {{0, 0}, false};
	}

	bool aggro = rand();

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

	return {_moves[favoredSon],info.first};
}

Move AI::getRandomMove() const {
	unsigned index = rand() % _children.size();

	return _moves[index];
}

