#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>

#include "bst.hpp"
#include "checkers.hpp"
#include "game.hpp"

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

GameTree::GameTree(unsigned level, const SaveGame record,
		const MoveRecord creator) :
		level(level), scenario(record, false, false), children(), p1Avg(
				scenario.getP1score()), p2Avg(scenario.getP2score()) {
	if (level > 1) {
		this->creator.dir = creator.dir;
		this->creator.jump = creator.jump;
		this->creator.piece = creator.piece;
		this->creator.prey = creator.prey;
	}
}

GameTree::~GameTree() {
	for (auto child : children)
		delete child;
}

void GameTree::printScene() {
	using namespace std;

	cout << "This is a level " << level << " Scenario with " << "P1 Min: "
			<< p1Avg << "\tP2 Min: " << p2Avg << endl;
	scenario.print();
}

bool GameTree::canMultiJump(unsigned piece) {
	bool retval;
	bool oldTurn = scenario.getTurn();
	scenario.setTurn(!oldTurn);
	SaveGame savestate = scenario.getSave();
	/* Test jumping */
	for (unsigned prey = 1; prey <= 12; prey++) {
		retval = scenario.jumpPiece(piece, prey);
		if (retval) {
			scenario.restoreToSave(savestate);
			scenario.setTurn(oldTurn);
			return true;
		}
	}

	scenario.setTurn(oldTurn);
	return retval;
}

unsigned GameTree::testMoves(SaveGame savestate) {
	using namespace std;

	bool retval;
	unsigned successCount = 0;

	MoveRecord origin;

	unsigned jumpCount = 0;
	for (unsigned piece = 1; piece <= 12; piece++) {
		/* Test jumping */
		for (unsigned prey = 1; prey <= 12; prey++) {
			retval = scenario.jumpPiece(piece, prey);
			if (retval) {
				++jumpCount;
				++successCount;
				origin.jump = true;
				origin.piece = piece;
				origin.prey = prey;
				if (canMultiJump(piece)) {
					scenario.setTurn(!scenario.getTurn());
				} else {
					scenario.setMustJump(0);
				}
				children.push_back(
						new GameTree(level + 1, scenario.getSave(), origin));
				scenario.restoreToSave(savestate);
			}
		}
	}
	if (jumpCount > 0)
		return successCount;

	for (unsigned piece = 1; piece <= 12; piece++) {

		/* Test left move */
		retval = scenario.movePiece(piece, Game::LEFT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = Game::LEFT;
			children.push_back(
					new GameTree(level + 1, scenario.getSave(), origin));
			scenario.restoreToSave(savestate);
		}

		/* Test right move */
		retval = scenario.movePiece(piece, Game::RIGHT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = Game::RIGHT;
			children.push_back(
					new GameTree(level + 1, scenario.getSave(), origin));
			scenario.restoreToSave(savestate);
		}

		/* Test back right move */
		retval = scenario.movePiece(piece, Game::BKRIGHT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = Game::BKRIGHT;
			children.push_back(
					new GameTree(level + 1, scenario.getSave(), origin));
			scenario.restoreToSave(savestate);
		}

		/* Test back left move */
		retval = scenario.movePiece(piece, Game::BKLEFT);
		if (retval) {
			++successCount;
			origin.jump = false;
			origin.piece = piece;
			origin.dir = Game::BKLEFT;
			children.push_back(
					new GameTree(level + 1, scenario.getSave(), origin));
			scenario.restoreToSave(savestate);
		}

	}
	return successCount;
}

void GameTree::generateOutcomes() {

	if (level > 5)
		return;

	SaveGame savestate = scenario.getSave();
	unsigned num = testMoves(savestate);
	if (num < 1)
		return;

	size_t numKids = children.size();
	for (size_t i = 0; i < numKids; i++) {
		children[i]->generateOutcomes();
	}
}

std::vector<GameTree *> GameTree::kidnap() {
	return children;
}

void GameTree::updateScores() {
	if (children.empty())
		return;

	size_t numKids = children.size();
	for (size_t i = 0; i < numKids; i++) {
		children[i]->updateScores();
	}

	double temp1 = 0, temp2 = 0;
	for (auto& child : children) {
		temp1 += child->p1Avg;
		temp2 += child->p2Avg;
	}

	p1Avg = temp1 / children.size();
	p2Avg = temp2 / children.size();
}

void GameTree::recursivePrint() {
	printScene();
	if (children.empty())
		return;

	for (auto& child : children) {
		child->recursivePrint();
	}
}

MoveRecord GameTree::getBestMove(bool optimizeForP2, bool aggro) {
	size_t nKids = children.size();

	if (nKids == 0) {
		std::cerr << "No Moves.\n";
		creator.piece =  0xff;
		return creator;
	}

	size_t favoredSon = 0;
	double bestAvg = 0.0;
	if (!aggro) {
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p2Avg > bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p1Avg > bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		}
	} else {
		bestAvg = 999999999;
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p1Avg < bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++) {
				if (children[i]->p2Avg < bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		}
	}
	return children[favoredSon]->getCreator();
}

void playPvP(Game *theGame) {
	using namespace std;

	string instring;
	unsigned count1 = 0, count2 = 0;
	MoveRecord blank;
	GameTree* predictor;

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
					predictor = new GameTree(1, theGame->getSave(), blank);
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
					predictor = new GameTree(1, theGame->getSave(), blank);
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
		GameTree* predictor, bool p1Turn) {
	using namespace std;

	unsigned count2 = 0;
	string player = (p1Turn ? " Player 1 " : " Player 2 ");

	do {
		if (interact)
			theGame->print();
		if (interact) {
			cout << player << "pieces = " << theGame->getP1score() << endl;
			cout << "================ " << player << " ==============\n\n";
		}

		if (count2 == 3) {
			count2 = 0;
			if (interact)
				cout << "Three failed moves" << player << "; Lose a turn!\n\n";
			theGame->setTurn(!theGame->getTurn());
			break;
		}

		GameTree AI(1, theGame->getSave(), blank);
		AI.generateOutcomes();
		AI.updateScores();
		blank = AI.getBestMove();
		if (blank.piece == 0xff) return true;
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

		if (blank.jump) {
			predictor = new GameTree(1, theGame->getSave(), blank);
			if (predictor->canMultiJump(theGame->getMustJump())) {
				theGame->setTurn(!theGame->getTurn());
				continue;
			} else
				theGame->setMustJump(0);
			delete predictor;
		}
		break;
	} while (1);

	return false;
}

void playAgainstAI(Game *theGame, bool interact) {
	using namespace std;

	MoveRecord blank;
	string instring;
	unsigned count1 = 0;
	GameTree *predictor;

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
					predictor = new GameTree(1, theGame->getSave(), blank);
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
			if (aiInteract(theGame, interact, blank, predictor, turn)) return;
		}
	}
}

void playAIvsAI(Game *theGame, bool interact) {
	using namespace std;

	MoveRecord blank;
	string instring;
	GameTree *predictor = NULL;
	bool turn;

	theGame->print();

	while (1) {
		if (interact)
			cout << theGame->getP1score() << " Player 1\n"
					<< theGame->getP2score() << " Player 2\n\n";

		if ((turn = theGame->getTurn())) {
			if (theGame->getP1score() < 1)
				return;

			if (aiInteract(theGame, interact, blank, predictor, turn)) return;
			delay(500);
		} else {
			if (theGame->getP2score() < 1)
				return;

			if (aiInteract(theGame, interact, blank, predictor, turn)) return;
			delay(500);
		}
	}

}
