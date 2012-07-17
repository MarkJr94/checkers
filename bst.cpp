#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <omp.h>

#include "bst.hpp"
#include "checkers.hpp"
#include "player.hpp"

GameTree::GameTree(unsigned level,const SaveGame record)
		: level(level), scenario (record, false), children (),
		  p1Min (scenario.getP1score()), p2Min (scenario.getP2Score())
{
}

void GameTree::printScene()
{
	using namespace std;

	cout << "This is a level " << level << " Scenario with " <<
			"P1 Min: " << p1Min << "\tP2 Min: " << p2Min << endl;
	scenario.print();
}


unsigned GameTree::testMoves(SaveGame savestate)
{
	using namespace std;

	bool retval;
	unsigned successCount = 0;

	for (unsigned piece = 1; piece <= 12; piece++) {
		/* Test left move */
		retval = scenario.movePiece(piece,Match::LEFT);
		if (retval) {
			++successCount;
			children.push_back(new GameTree (level+1, scenario.getSave()));
			scenario.restoreToSave(savestate);
		}

		/* Test right move */
		retval = scenario.movePiece(piece, Match::RIGHT);
		if (retval) {
			++successCount;
			children.push_back(new GameTree (level+1, scenario.getSave()));
			scenario.restoreToSave(savestate);
		}

		/* Test jumping */
		for (unsigned prey = 1; prey <= 12; prey++) {
			retval = scenario.jumpPiece(piece,prey);
			if (retval) {
				++successCount;
				children.push_back(new GameTree (level+1, scenario.getSave()));
				scenario.restoreToSave(savestate);
			}
		}
	}
	return successCount;
}

void GameTree::recurse()
{

	if (level > 6) return;
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
		temp1 += child->p1Min;
		temp2 += child->p2Min;
	}

	p1Min = temp1/children.size();
	p2Min = temp1/children.size();
}

void GameTree::recursivePrint()
{
	printScene();
	if (children.empty()) return;

	for (auto& child : children) {
		child->recursivePrint();
	}
}

void gameDo()
{
	using namespace std;

	GameTree tree (1,Match(false).getSave());
//	tree.printScene();
	tree.recurse();
	tree.updateScores();
//	tree.recursivePrint();
}

int main ()
{
	using namespace std;

	gameDo();
	return 0;
}
