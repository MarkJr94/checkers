/*
 * GameMaster.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: markjr
 */

#include <iostream>

#include "GameMaster.hpp"

/* Sleep portability thing */
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>

inline void delay( const unsigned long ms )
{
	Sleep( ms );
}

#else  /* presume POSIX */

#include <unistd.h>

inline void delay(const unsigned long ms) {
	usleep(ms * 1000);
}

#endif

GameMaster::GameMaster(const bool interact) :
		_game(new Game(false, interact)), _ai(new AI(0, Save())), _interact(
				interact), _table() {
	// TODO Auto-generated constructor stub

}

GameMaster::~GameMaster() {
	delete _ai;
	delete _game;
	// TODO Auto-generated destructor stub
}

void GameMaster::setDifficulty(const unsigned difficulty) {
	if (difficulty > 10)
		_ai->_difficulty = difficulty % 10;
	else
		_ai->_difficulty = difficulty;
}

bool GameMaster::aiInteract() {
	using namespace std;

	bool p1Turn = _game->_turn;
	unsigned count2 = 0;
	string player = (p1Turn ? " Player 1 " : " Player 2 ");
	bool success;

	do {
		if (_interact)
			_game->print();
		if (_interact) {
			unsigned score =
					(p1Turn ? _game->getP1score() : _game->getP2score());
			cout << player << "pieces = " << score << endl;
			cout << "================ " << player << " ==============\n\n";
		}

		if (count2 == 3) {
			count2 = 0;
			if (_interact)
				cout << "Three failed moves" << player << "; Lose a turn!\n\n";
			_game->_turn = !_game->_turn;
			break;
		}

		_move = _ai->evaluateGame(*_game);
		if (_move.piece == 0xff)
			return true;

		const Save oldsave = _game->getSave();
		success = _game->makeMove(_move);

		Hash::Zkey hash = _game->getHash();
		if (_table[hash]++) {
			_game->restoreToSave(oldsave);
			_game->makeMove(_ai->getRandomMove());
		}

		if (!success) {
			count2++;
			continue;
		}

		if (_move.jump) {

			if (_ai->canMultiJump(*_game)) {
				_game->_turn = !_game->_turn;
				continue;
			} else
				_game->_mustJump = 0;
		}
		break;
	} while (1);

	return false;
}

void GameMaster::playAIvsAI() {
	using namespace std;

	bool turn;

//	_game->print();

	while (1) {
//		if (_interact)
//			cout << _game->getP1score() << " Player 1\n" << _game->getP2score()
//					<< " Player 2\n\n";

		if ((turn = _game->_turn)) {
			if (_game->getP1score() < 1)
				return;

			if (aiInteract())
				return;
//			delay(500);
		} else {
			if (_game->getP2score() < 1)
				return;

			if (aiInteract())
				return;
//			delay(500);
		}
	}
}

void GameMaster::playPvP() {

	using namespace std;

	unsigned count1 = 0, count2 = 0;

	_game->print();

	while (1) {
		cout << _game->getP1score() << " Player 1\n" << _game->getP2score()
				<< " Player 2\n\n";

		if (_game->getTurn()) {
			if (_game->getP1score() < 1)
				return;
			cout << "P1 pieces = " << _game->getP1score() << endl;
			while (1) {
				_game->print();
				if (count1 == 3) {
					count1 = 0;
					cout << "Three failed moves P1; Lose a turn!\n\n";
					_game->setTurn(!_game->getTurn());
					break;
				}
				cout << "================ Player 1 (Black) ==============\n\n";
				int c = _game->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count1++;
					continue;
				case 2:
					if (_ai->canMultiJump(*_game)) {
						_game->_turn = !_game->_turn;
					}
				default:
					_game->_mustJump = 0;
					break;
				}
				count1 = 0;
				break;
			}
		} else {
			if (_game->getP2score() < 1)
				return;
			cout << "P2 pieces = " << _game->getP2score() << endl;
			while (1) {
				_game->print();
				if (count2 == 3) {
					count2 = 0;
					cout << "Three failed moves P2; Lose a turn!\n\n";
					_game->_turn = !_game->_turn;
					break;
				}
				cout << "================ Player 2 (Red) ==============\n\n";
				int c = _game->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count2++;
					continue;
				case 2:
					if (_ai->canMultiJump(*_game)) {
						_game->_turn = !_game->_turn;
						continue;
					}
				default:
					_game->setMustJump(0);
					break;
				}
				count2 = 0;
				break;
			}
		}
	}
}

void GameMaster::playAgainstAI() {
	using namespace std;

	unsigned count1 = 0;

	_game->print();
	bool turn;

	while (1) {
		if (_interact)
			cout << _game->getP1score() << " Player 1\n" << _game->getP2score()
					<< " Player 2\n\n";

		if ((turn = _game->getTurn())) {
			if (_game->getP1score() < 1)
				return;
			cout << "P1 pieces = " << _game->getP2score() << endl;
			while (1) {
				_game->print();
				if (count1 == 3) {
					count1 = 0;
					cout << "Three failed moves P1; Lose a turn!\n\n";
					_game->_turn = !_game->_turn;
					break;
				}
				cout << "================ Player 1 (Black) ==============\n\n";
				int c = _game->receiveInput();
				switch (c) {
				case -1:
					return;
				case 0:
					count1++;
					continue;
				case 2:
					if (_ai->canMultiJump(*_game)) {
						_game->_turn = !_game->_turn;
						continue;
					}
				default:
					_game->setMustJump(0);
					break;
				}
				count1 = 0;
				break;
			}
		} else {
			if (_game->getP2score() < 1)
				return;
			if (aiInteract())
				return;
		}
	}
}

void GameMaster::testGame() {
	bool success;
	unsigned numErrors = 0;

	do {
		_game->print();
		_move = _ai->evaluateGame(*_game);
		std::cout << "Chosen Move: \n";
		printMove(_move);
		success = _game->makeMove(_move);

		if (!success) {
			std::cerr << "AN ERROR OCCURED\n";
			return;
		}
	} while (_game->getP1score() * _game->getP2score() > 0 && numErrors == 0);

	std::cerr << "EXITING NORMALLY\n";
}

