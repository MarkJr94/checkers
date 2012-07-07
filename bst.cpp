#include <vector>
#include <iostream>
#include "checkers.hpp"
#include "player.hpp"
#include "bst.hpp"

GameTree::GameTree(bool turn)
		: myScore (12), otherScore(12), p1 (Piece::BLACK,false), 
			p2 (Piece::RED,false), turn(turn),
			record (turn)
{
	using namespace std;
	
	auto alias = *( p1.getGame());
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		for (unsigned j = 0; j < BOARD_SIZE; j++) {
			if (alias[i][j].getInPlay()) {
				record(i,j).alive = true;
				record(i,j).id = alias[i][j].id;
				record(i,j).color = alias[i][j].getColor();
			}
		}
	}
}

GameTree::GameTree(const SaveGame& recordd)
		: p1 (Piece::BLACK, recordd, false), p2 (Piece::RED, recordd, false),
			turn (false) , record(false)
{
	this->record = recordd;
	myScore = p1.getnPieces();
	otherScore = p2.getnPieces();
	std::cout << "Board created from record\n\n";
}
	
void GameTree::printRecord()
{
	using namespace std;
	cout << endl;
	for (int j = 7; j >= 0; j--) {
		for (unsigned i = 0; i < BOARD_SIZE; i++) {
			if (record(i,j).alive == true) cout << "O";
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
			record(oldx,oldy).alive = false;
			unsigned newx = alias->getX();
			unsigned newy = alias->getY();
			record(newx,newy).alive = true;
			record(newx,newy).id = i;
			record(newx,newy).color = alias->getColor();
			break;
		}
	}
	turn = !turn;
	GameTree child (record);
	child.printRecord();
}
			

int main ()
{
	using namespace std;
	GameTree tree;
	tree.printRecord();
	for (unsigned i = 0; i < 12; i++) {
		tree.test(i);
	}
	tree.printRecord();
	cout << "SUCCESS!" << endl;
	return 0;
}
