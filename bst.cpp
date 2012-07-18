#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <omp.h>
#include <sstream>

#include "bst.hpp"
#include "checkers.hpp"
#include "player.hpp"

GameTree::GameTree(unsigned level,const SaveGame record, const MoveRecord creator)
		: level(level), scenario (record, false, false), children (),
		  p1Avg (scenario.getP1score()), p2Avg (scenario.getP2score())
{
	if (level > 1){
		this->creator.dir = creator.dir;
		this->creator.jump = creator.jump;
		this->creator.piece = creator.piece;
		this->creator.prey = creator.prey;
	}
}

void GameTree::printScene()
{
	using namespace std;

	cout << "This is a level " << level << " Scenario with " <<
			"P1 Min: " << p1Avg << "\tP2 Min: " << p2Avg << endl;
	scenario.print();
}


unsigned GameTree::testMoves(SaveGame savestate)
{
	using namespace std;

	bool retval;
	unsigned successCount = 0;

	MoveRecord origin;
	srand(time(NULL));
	unsigned maybeRand;
	unsigned mask1 = 1 << 8;
	unsigned mask2 = 1 << 6;
	unsigned mask3 = 1 << 4;
	unsigned mask4 = 1 << 2;

	for (unsigned piece = 1; piece <= 12; piece++) {
		maybeRand = rand() %2;
		/* Test left move */
		if (!(mask1 & maybeRand)) {
			retval = scenario.movePiece(piece,Match::LEFT);
			if (retval) {
				++successCount;
				origin.jump = false;
				origin.piece = piece;
				origin.dir = Match::LEFT;
				children.push_back(new GameTree (level+1, scenario.getSave(),origin));
				scenario.restoreToSave(savestate);
			}
		}
		if (!(mask2 & maybeRand)) {
			/* Test right move */
			retval = scenario.movePiece(piece, Match::RIGHT);
			if (retval) {
				++successCount;
				origin.jump = false;
				origin.piece = piece;
				origin.dir = Match::RIGHT;
				children.push_back(new GameTree (level+1, scenario.getSave(),origin));
				scenario.restoreToSave(savestate);
			}
		}

//		maybeRand = rand() %2;
		if (!(mask3 & maybeRand)) {
			/* Test back right move */
			retval = scenario.movePiece(piece, Match::BKRIGHT);
			if (retval) {
				++successCount;
				origin.jump = false;
				origin.piece = piece;
				origin.dir = Match::BKRIGHT;
				children.push_back(new GameTree (level+1, scenario.getSave(),origin));
				scenario.restoreToSave(savestate);
			}
		}
		if (!(mask4 & maybeRand)) {
			/* Test back left move */
			retval = scenario.movePiece(piece, Match::BKLEFT);
			if (retval) {
				++successCount;
				origin.jump = false;
				origin.piece = piece;
				origin.dir = Match::BKLEFT;
				children.push_back(new GameTree (level+1, scenario.getSave(),origin));
				scenario.restoreToSave(savestate);
			}
		}

		/* Test jumping */
		for (unsigned prey = 1; prey <= 12; prey++) {
			retval = scenario.jumpPiece(piece,prey);
			if (retval) {
				++successCount;
				origin.jump = true;
				origin.piece = piece;
				origin.prey = prey;
				children.push_back(new GameTree (level+1, scenario.getSave(), origin));
				scenario.restoreToSave(savestate);
			}
		}
	}
	return successCount;
}

void GameTree::recurse()
{

	srand(time(NULL));
	if (rand() % 2) {
		if (level > 5) return;
	} else
		if (level > 4) return;

	SaveGame savestate = scenario.getSave();
	unsigned num =  testMoves(savestate);
	if (num < 1) return;

	size_t numKids = children.size();
	#pragma omp parallel for
	for (size_t i = 0; i < numKids; i++) {
		children[i]->recurse();
	}
}

std::vector<GameTree *> GameTree::kidnap() { return children; }

void GameTree::updateScores()
{
	if (children.empty()) return;

	size_t numKids = children.size();
	#pragma omp parallel for
	for (size_t i = 0; i < numKids; i++) {
		children[i]->updateScores();
	}

	double temp1 = 0, temp2 = 0;
	for (auto& child : children) {
		temp1 += child->p1Avg;
		temp2 += child->p2Avg;
	}

	p1Avg = temp1/children.size();
	p2Avg = temp2/children.size();
}

void GameTree::recursivePrint()
{
	printScene();
	if (children.empty()) return;

	for (auto& child : children) {
		child->recursivePrint();
	}
}

MoveRecord GameTree::getBestMove(bool optimizeForP2, bool aggro)
{
	size_t nKids = children.size();

	if (nKids == 0) {
		std::cerr << "No Moves.\n";
		return creator;
	}

	size_t favoredSon = 0;
	double bestAvg = 0.0;
	if (!aggro) {
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++){
				if (children[i]->p2Avg > bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++){
				if (children[i]->p1Avg > bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		}
	} else {
		bestAvg = 999999999;
		if (optimizeForP2) {
			for (size_t i = 0; i < nKids; i++){
				if (children[i]->p1Avg < bestAvg) {
					bestAvg = children[i]->p1Avg;
					favoredSon = i;
				}
			}
		} else {
			for (size_t i = 0; i < nKids; i++){
				if (children[i]->p2Avg < bestAvg) {
					bestAvg = children[i]->p2Avg;
					favoredSon = i;
				}
			}
		}
	}

//	children[favoredSon]->printScene();
	return children[favoredSon]->getCreator();
}

void gameDo()
{
	using namespace std;

	MoveRecord blank;
	GameTree tree (1,Match(false,false).getSave(),blank);
	tree.recurse();
	tree.updateScores();
	tree.getBestMove();
}

void attemptToPlay(bool debug)
{
	using namespace std;

	MoveRecord blank;
	Match theGame (false,true);
	unsigned prey;
	unsigned piece;
	Match::Direction d;
	string instring;
	unsigned count1 = 0, count2 = 0;

	theGame.print();

	while (1) {
		if (debug) cout << theGame.getP1score() << " Player 1\n" << theGame.getP2score() << " Player 2\n\n";

		if (theGame.getTurn()) {
			if (theGame.getP1score() < 1) return;
			if (debug) cout << "P1 pieces = " << theGame.getP1score() << endl;
			while (1) {
				if (debug) theGame.print();
				if (count1 == 3) {
					count1 = 0;
					if (debug) cout << "Three failed moves P1; Lose a turn!\n\n";
					theGame.setTurn(!theGame.getTurn());
					break;
				}
				if (debug) cout << "================ Player 1 (Black) ==============\n\n";
				int c = theGame.receiveInput(piece,d);
				switch (c) {
					case -1: return;
					case 0: {
						count1++;
						continue;
					}
					default:;
				}
				count1 = 0;
				break;
			}
		} else {
			if (debug) theGame.print();
			if (theGame.getP2score() <1) return;
			if (debug) cout << "P2 pieces = " << theGame.getP2score() << endl;
			if (debug) cout << "================ Player 2  (Red) ==============\n\n";

			if (count2 == 3) {
				count2 = 0;
				if (debug) cout << "Three failed moves P2; Lose a turn!\n\n";
				theGame.setTurn(!theGame.getTurn());
				break;
			}


			GameTree AI (1,theGame.getSave(),blank);
			AI.recurse();
			AI.updateScores();
			blank = AI.getBestMove();
			bool success;
			if (blank.jump) {
				success = theGame.jumpPiece(blank.piece,blank.prey);
			} else {
				success = theGame.movePiece(blank.piece,blank.dir);
			}

			if (!success) {
				count2++;
				continue;
			}
		}
	}

}

void simulate(bool debug)
{
	using namespace std;

	MoveRecord blank;
	Match theGame (false,false);
	unsigned prey;
	unsigned piece;
	Match::Direction d;
	string instring;
	unsigned count1 = 0, count2 = 0;

	theGame.print();

	while (1) {
		if (debug) cout << theGame.getP1score() << " Player 1\n" << theGame.getP2score() << " Player 2\n\n";

		if (theGame.getTurn()) {
			if (debug) theGame.print();
			if (theGame.getP1score() <1) return;
			if (debug) cout << "P1 pieces = " << theGame.getP2score() << endl;
			if (debug) cout << "================ Player 1  (Black) ==============\n\n";

			if (count2 == 3) {
				count2 = 0;
				if (debug) cout << "Three failed moves P1; Lose a turn!\n\n";
				theGame.setTurn(!theGame.getTurn());
				break;
			}


			GameTree AI (1,theGame.getSave(),blank);
			AI.recurse();
			AI.updateScores();
			blank = AI.getBestMove(false,true);
			bool success;
			if (blank.jump) {
				success = theGame.jumpPiece(blank.piece,blank.prey);
			} else {
				success = theGame.movePiece(blank.piece,blank.dir);
			}

			if (!success) {
				count2++;
				continue;
			}
		} else {
			if (debug) theGame.print();
			if (theGame.getP2score() <1) return;
			if (debug) cout << "P2 pieces = " << theGame.getP2score() << endl;
			if (debug) cout << "================ Player 2  (Red) ==============\n\n";

			if (count2 == 3) {
				count2 = 0;
				if (debug) cout << "Three failed moves P2; Lose a turn!\n\n";
				theGame.setTurn(!theGame.getTurn());
				break;
			}


			GameTree AI (1,theGame.getSave(),blank);
			AI.recurse();
			AI.updateScores();
			blank = AI.getBestMove();
			bool success;
			if (blank.jump) {
				success = theGame.jumpPiece(blank.piece,blank.prey);
			} else {
				success = theGame.movePiece(blank.piece,blank.dir);
			}

			if (!success) {
				count2++;
				continue;
			}
		}
	}

}

int main ()
{
	using namespace std;

//	gameDo();
	attemptToPlay(true);
//	simulate(true);
	return 0;
}
