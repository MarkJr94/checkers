/*
 * AI.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: msinclair94
 */
#include <iostream>

#include "AI.hpp"

/* Sleep portability thing */
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>

inline void delay( unsigned long ms )
{
	Sleep( ms );
}

#else  /* presume POSIX */

#include <unistd.h>

inline void delay(unsigned long ms) {
	usleep(ms * 1000);
}

#endif

Game* AI::_game = new Game(false, false);

AI::AI(unsigned degree, const Save& save, const MoveRecord& creator,
		const unsigned difficulty) :
		_level(degree), _difficulty(difficulty), _children(), _move(creator), _save(
				save) {

}

AI::~AI() {
	for (auto child : _children)
		delete child;

}

void AI::resetToSave(const Save& save) {
	_game->restoreToSave(save);
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

	MoveRecord origin;
//	_game->restoreToSave(_save);
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
//				printMove(origin);
//				if (canMultiJump(piece)) {
//					scenario->setTurn(!scenario->getTurn());
//				} else {
				_game->setMustJump(0);
//				}
				_children.push_back(
						new AI(_level + 1, _game->getSave(), origin));
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
			_children.push_back(new AI(_level + 1, _game->getSave(), origin));
		}

		/* Test right move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, RIGHT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = RIGHT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin));
		}

		/* Test back right move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, BKRIGHT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = BKRIGHT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin));
		}

		/* Test back left move */
		_game->restoreToSave(_save);
		retval = _game->movePiece(piece, BKLEFT);
		if (retval) {
			++successCount;
			origin.piece = piece;
			origin.dir = BKLEFT;
			_children.push_back(new AI(_level + 1, _game->getSave(), origin));
		}

	}
	return successCount;
}

unsigned AI::generateOutcomes() {

	if (_level == _difficulty - 1)
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

MoveRecord AI::evaluateMoves(bool optimizeForP1, bool aggro) {

	unsigned nKids = generateOutcomes();
	updateScores();

	if (nKids == 0) {
		std::cerr << "NO MOVES.\n";
		_move.piece = 0xff;
		return _move;
	}

	std::cout << "Number of moves:  " << nKids << std::endl;

	unsigned favoredSon = 0;
	float bestAvg = 0.0;
	if (!aggro) {
		if (!optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				printMove(_children[i]->_move);
				if (_children[i]->_p2Avg > bestAvg) {
					bestAvg = _children[i]->_p2Avg;
					favoredSon = i;
				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				printMove(_children[i]->_move);
				if (_children[i]->_p1Avg > bestAvg) {
					bestAvg = _children[i]->_p1Avg;
					favoredSon = i;
				}
			}
		}
	} else {
		bestAvg = 999999999;
		if (!optimizeForP1) {
			for (unsigned i = 0; i < nKids; i++) {
				printMove(_children[i]->_move);
				if (_children[i]->_p1Avg < bestAvg) {
					bestAvg = _children[i]->_p1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (unsigned i = 0; i < nKids; i++) {
				printMove(_children[i]->_move);
				if (_children[i]->_p2Avg < bestAvg) {
					bestAvg = _children[i]->_p2Avg;
					favoredSon = i;

				}
			}
		}
	}

	return _children[favoredSon]->_move;
}

MoveRecord AI::evaluateGame(Game& game) {
	resetToSave(game.getSave());

	return evaluateMoves(game.getTurn(), true);
}

bool AI::canMultiJump(unsigned piece) {
	bool retval = false;
	_game->setTurn(!_game->getTurn());
	Save savestate = _game->getSave();
	/* Test jumping */
	for (unsigned prey = 1; prey <= 12; prey++) {
		retval = _game->jumpPiece(piece, prey);
		if (retval) {
			_game->restoreToSave(savestate);
			return true;
		}
	}

	_game->restoreToSave(savestate);
	return retval;
}

void playPvP(Game *theGame) {
	using namespace std;

	string instring;
	unsigned count1 = 0, count2 = 0;
	MoveRecord blank;
	AI* predictor;

	theGame->print();

	while (1) {
		cout << theGame->getP1score() << " Player 1\n" << theGame->getP2score()
				<< " Player 2\n\n";

		if (theGame->getTurn()) {
			if (theGame->getP1score() < 1)
				return;
			cout << "P1 pieces = " << theGame->getP1score() << endl;
			while (1) {
				theGame->print();
				if (count1 == 3) {
					count1 = 0;
					cout << "Three failed moves P1; Lose a turn!\n\n";
					theGame->setTurn(!theGame->getTurn());
					break;
				}
				cout << "================ Player 1 (Black) ==============\n\n";
				int c = theGame->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count1++;
					continue;
				case 2:
					predictor = new AI(1, theGame->getSave(), blank);
					if (predictor->canMultiJump(theGame->getMustJump())) {
						theGame->setTurn(!theGame->getTurn());
						continue;
					}
					delete predictor;
				default:
					theGame->setMustJump(0);
					break;
				}
				count1 = 0;
				break;
			}
		} else {
			if (theGame->getP2score() < 1)
				return;
			cout << "P2 pieces = " << theGame->getP2score() << endl;
			while (1) {
				theGame->print();
				if (count2 == 3) {
					count2 = 0;
					cout << "Three failed moves P2; Lose a turn!\n\n";
					theGame->setTurn(!theGame->getTurn());
					break;
				}
				cout << "================ Player 2 (Red) ==============\n\n";
				int c = theGame->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count2++;
					continue;
				case 2:
					predictor = new AI(1, theGame->getSave(), blank);
					if (predictor->canMultiJump(theGame->getMustJump())) {
						theGame->setTurn(!theGame->getTurn());
						continue;
					}
					delete predictor;
				default:
					theGame->setMustJump(0);
					break;
				}
				count2 = 0;
				break;
			}
		}
	}
}

bool aiInteract(Game *theGame, const bool interact, MoveRecord& blank,
		AI* predictor, bool p1Turn) {
	using namespace std;

	unsigned count2 = 0;
	string player = (p1Turn ? " Player 1 " : " Player 2 ");

	do {
		if (interact)
			theGame->print();
		if (interact) {
			unsigned score = (
					p1Turn ? theGame->getP1score() : theGame->getP2score());
			cout << player << "pieces = " << score << endl;
			cout << "================ " << player << " ==============\n\n";
		}

		if (count2 == 3) {
			count2 = 0;
			if (interact)
				cout << "Three failed moves" << player << "; Lose a turn!\n\n";
			theGame->setTurn(!theGame->getTurn());
			break;
		}

		AI ai(1, theGame->getSave(), blank);
		blank = ai.evaluateGame(*theGame);
		if (blank.piece == 0xff)
			return true;
		bool success;
		if (blank.jump) {
			success = theGame->jumpPiece(blank.piece, blank.prey);
		} else {
			success = theGame->movePiece(blank.piece, blank.dir);
		}

		if (!success) {
			count2++;
			continue;
		}

//		if (blank.jump) {
//			predictor = new AI(1, theGame->getSave(), blank);
//			if (predictor->canMultiJump(theGame->getMustJump())) {
//				theGame->setTurn(!theGame->getTurn());
//				continue;
//			} else
//				theGame->setMustJump(0);
//			delete predictor;
//		}
		break;
	} while (1);

	return false;
}

void playAgainstAI(Game *theGame, bool interact) {
	using namespace std;

	MoveRecord blank;
	string instring;
	unsigned count1 = 0;
	AI *predictor;

	theGame->print();
	bool turn;

	while (1) {
		if (interact)
			cout << theGame->getP1score() << " Player 1\n"
					<< theGame->getP2score() << " Player 2\n\n";

		if ((turn = theGame->getTurn())) {
			if (theGame->getP1score() < 1)
				return;
			cout << "P1 pieces = " << theGame->getP2score() << endl;
			while (1) {
				theGame->print();
				if (count1 == 3) {
					count1 = 0;
					cout << "Three failed moves P1; Lose a turn!\n\n";
					theGame->setTurn(!theGame->getTurn());
					break;
				}
				cout << "================ Player 1 (Black) ==============\n\n";
				int c = theGame->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count1++;
					continue;
				case 2:
					predictor = new AI(1, theGame->getSave(), blank);
					if (predictor->canMultiJump(theGame->getMustJump())) {
						theGame->setTurn(!theGame->getTurn());
						continue;
					}
					delete predictor;
				default:
					theGame->setMustJump(0);
					break;
				}
				count1 = 0;
				break;
			}
		} else {
			if (theGame->getP2score() < 1)
				return;
			if (aiInteract(theGame, interact, blank, predictor, turn))
				return;
		}
	}
}

void playAIvsAI(Game *theGame, bool interact) {
	using namespace std;

	MoveRecord blank;
	string instring;
	AI *predictor = NULL;
	bool turn;

	theGame->print();

	while (1) {
		if (interact)
			cout << theGame->getP1score() << " Player 1\n"
					<< theGame->getP2score() << " Player 2\n\n";

		if ((turn = theGame->getTurn())) {
			if (theGame->getP1score() < 1)
				return;

			if (aiInteract(theGame, interact, blank, predictor, turn))
				return;
//			delay(500);
		} else {
			if (theGame->getP2score() < 1)
				return;

			if (aiInteract(theGame, interact, blank, predictor, turn))
				return;
//			delay(500);
		}
	}

}

void printMove(const MoveRecord& move) {
	using std::cout;

	if (move.jump) {
		cout << "Piece No. (" << (int) move.piece << ") preying on Piece No. ("
				<< (int) move.prey << ")" << std::endl;
		return;
	}

	cout << "Piece No. (" << (int) move.piece << ")"
			<< " moving in direction: ";
	switch (move.dir) {
	case LEFT:
		cout << "LEFT";
		break;
	case RIGHT:
		cout << "RIGHT";
		break;
	case BKRIGHT:
		cout << "BKRIGHT";
		break;
	case BKLEFT:
		cout << "BKLEFT";
		break;
	}
	cout << ".\n";
}
