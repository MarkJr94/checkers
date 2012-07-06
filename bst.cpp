#include <vector>
#include <iostream>
#include "checkers.hpp"
#include "player.hpp"
#include "bst.hpp"

GameTree::GameTree(bool turn)
		: myScore (12), otherScore(12), p1 (Piece::BLACK,false), 
			p2 (Piece::RED,false), turn(turn), 
			record (8,std::vector<bool> (8, false))
{
	using namespace std;
	
	auto alias = *( p1.getGame());
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			if (alias[i][j].getInPlay())
				record[i][j] = true;
		}
	}
}

void GameTree::printRecord()
{
	using namespace std;
	for (int j = 7; j >= 0; j--) {
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			if (record[i][j] == true) cout << "O";
			else cout << "-";
		}
		cout << endl;
	}
}

void GameTree::test(unsigned piece)
{
	Player *player;
	if (turn) player = &p1;
	else player = &p2;
	auto &pieces = *(player->getPieces());
	for (unsigned i = 0; i < 12; i++) {
		auto &alias = pieces[i];
		unsigned oldx = alias->getX();
		unsigned oldy = alias->getY();
		if (player->movePiece(i, Player::LEFT) || player->movePiece(i,Player::RIGHT)) {
			record[oldx][oldy] = false;
			record[alias->getX()][alias->getY()] = true;
			break;
		}
	}
	turn = !turn;
}
			

int main ()
{
	using namespace std;
	GameTree tree;
	tree.printRecord();
	tree.test(1);
	tree.printRecord();
	cout << "SUCCESS!" << endl;
	return 0;
}
