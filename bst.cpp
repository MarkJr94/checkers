#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "bst.hpp"
#include "checkers.hpp"
#include "player.hpp"

GameTree::GameTree(unsigned level,const SaveGame record)
		: level(level), scenario (record, false), children ()
{}

void GameTree::printScene()
{
	using namespace std;

	cout << "This is a level " << level << " Scenario\n";
	cout << "With " << children.size() << " children.\n\n";
	scenario.print();
}


unsigned GameTree::testMoves(SaveGame savestate)
{
	using namespace std;

	bool retval;
	int maybeRandom;
	srand(time(NULL));
	unsigned successCount = 0;

	vector<unsigned> numvec;
	for (unsigned i = 1; i <= 12; i++)
		numvec.push_back(i);

	random_shuffle(numvec.begin(),numvec.end());
	for (unsigned piece : numvec) {
		bool turnn = scenario.getSave().getTurn();
//		cout << "Testing piece " << piece << " with turn = " << turnn << endl;
		cout << " Turn = " << turnn << " of a level " << level << "scenario" << endl;
		maybeRandom = rand() % 2;
		if (maybeRandom) {
			retval = scenario.movePiece(piece,Match::LEFT);
			if (!retval) retval = scenario.movePiece(piece, Match::RIGHT);
		} else {
			retval = scenario.movePiece(piece,Match::RIGHT);
			if (!retval) retval =  scenario.movePiece(piece,Match::LEFT);
		}

		if (retval) {
			++successCount;
			GameTree child(level+1, scenario.getSave());
			children.push_back(child);
			scenario.restoreToSave(savestate);
			continue;
		}

		retval = testJumps(piece,savestate);
		if (retval) {
			++successCount;
			GameTree child(level+1, scenario.getSave());
			children.push_back(child);
			scenario.restoreToSave(savestate);
			continue;
		}
	}
	return successCount;

}

bool GameTree::testJumps(unsigned i, SaveGame savestate)
{
	using namespace std;

	vector<unsigned> numvec;
	for (unsigned i = 1; i <= 12; i++)
		numvec.push_back(i);

	random_shuffle(numvec.begin(),numvec.end());
	for (auto prey: numvec) {
		if (scenario.jumpPiece(i, prey)) {
			return true;
		}
	}

	return false;
}

void GameTree::generateChildren()
{
	if (level > 2) return;
	SaveGame savestate = scenario.getSave();
	unsigned numSucceeded = testMoves(savestate);
	std::cout << "numSucceeded = " << numSucceeded << std::endl << std::endl;
	if (numSucceeded < 1) {
		std::cout << "No available moves man :(\n\n";
		return;
	}

	for (auto& child : children) {
		child.printScene();
//		child.generateChildren();
	}
}

void GameTree::recurse()
{

	if (level > 3) return;
	SaveGame savestate = scenario.getSave();
	unsigned num =  testMoves(savestate);
	if (num < 1) return;

	for (auto& child : children) {
		child.printScene();
	}
}

int main ()
{
	using namespace std;

	GameTree tree (1,Match(false).getSave());
	tree.printScene();
	tree.recurse();
	vector<GameTree> future = tree.kidnap();
	GameTree tree2 (1,future[1].scenario.getSave());
	tree2.printScene();
	tree2.recurse();
	cout << "SUCCESS!" << endl;
	return 0;
}
