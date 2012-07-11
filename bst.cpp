#include <vector>
#include <iostream>
#include "checkers.hpp"
#include "player.hpp"
#include "bst.hpp"

GameTree::GameTree(const SaveGame& record)
		: scenario (record, false), children ()
{}

void GameTree::printScene()
{
	using namespace std;
	scenario.print();
}

void GameTree::test()
{
	for (unsigned i = 1; i <= 12; i++) {
		if (scenario.movePiece(i,Match::LEFT) ||
				scenario.movePiece(i,Match::RIGHT)) break;
	}
	GameTree child (scenario.getSave());
	child.printScene();
}

void GameTree::generateChildren()
{
	SaveGame savestate = scenario.getSave();

	for (unsigned i = 1; i <= 12; i++) {
		if ( scenario.movePiece(i,Match::LEFT)) {
			GameTree child (scenario.getSave());
			children.push_back(child);
			scenario.restoreToSave(savestate);
			continue;
		}
		if (scenario.movePiece(i,Match::RIGHT)) {
			GameTree child (scenario.getSave());
			children.push_back(child);
			scenario.restoreToSave(savestate);
			continue;
		}
//		if (success) break;
		for (unsigned j = 1; j <= 12; j++) {
			if (scenario.jumpPiece(i,j)) {
				GameTree child (scenario.getSave());
				children.push_back(child);
				scenario.restoreToSave(savestate);
				break;
			}
		}
	}

	for (auto& child : children) {
		child.printScene();
	}

}
			

int main ()
{
	using namespace std;
	GameTree tree (Match(false).getSave());
	tree.printScene();
	tree.generateChildren();
//	tree.printScene();
	cout << "SUCCESS!" << endl;
	return 0;
}
