#include <iostream>
#include <string>

#include "checkers.hpp"
#include "player.hpp"

int  receiveInput(Player& player, Player& pb, unsigned piece, Player::Direction d)
{
	using namespace std;
	string dirString;
	player.printgame();
	unsigned prey;
	
	cout << "Enter piece id: ";
	cin  >> piece;
	cout << "('q' = quit)\tEnter Direction 'l' = left or 'r' = right or 'j' = jump: ";
	cin >> dirString;
	if (dirString == "q") return -1;
	/* Jumping */
	if (dirString == "j") {
		cout << "Enter prey ID: ";
		cin >> prey;
		if (!player.jumpPiece(piece,prey,pb)) {
			cerr << "Jumping error; try again\n";
			return 0;
		}
		return 1;
	}
	
	/* Regular Movement */
	if  (dirString == "l")
		d = Player::LEFT;
	else if (dirString == "r")
		d = Player::RIGHT;
	else {
		cerr << "Input Error\n";
		return 0;
	}
	
	if (!player.movePiece(piece,d)) {
		cerr << "Movement error; try again\n";
		return 0;
	}
	return 1;
}
	
void dostuff() {
	using namespace std;
	
	Player player1 (Piece::BLACK);
	Player player2 (Piece::RED);
	player1.display(); player2.display();
	player1.printgame();
	
	unsigned count1 = 0,count2 = 0;
	unsigned piece;
	Player::Direction d;
	
	while (1) {
		cout << player1.numPieces << " Player 1\n" << player2.numPieces << " Player 2\n\n";
		if (player1.numPieces < 1) return;
		cout << "P1 pieces = " << player1.numPieces << endl;
		while (1) {
			if (count1 == 3) {
				count1 = 0;
				cerr << "Three failed moves P1; Lose a turn!\n\n";
				break;
			}
			cout << "================ Player 1 (Black) ==============\n\n";
			int c = receiveInput(player1,player2,piece,d);
			switch (c) {
				case -1: return;
				case 0: {
					count1++;
					continue;
				}
				default:;
			}
			count1 = 0;
			player1.display();
			player1.printgame();
			break;
		}
		if (player2.numPieces <1) return;
		cout << "P2 pieces = " << player2.numPieces << endl;
		while (1) {
			if (count2 == 3) {
				count2 = 0;
				cerr << "Three failed moves P2; Lose a turn!\n\n";
				break;
			}
			cout << "================ Player 2  (Red) ==============\n\n";
			int c = receiveInput(player2,player1,piece,d);
			switch (c) {
				case -1: return;
				case 0: {
					count2++;
					continue;
				}
				default:;
			}
			count2 = 0;
			player2.display();
			player2.printgame();
			break;
		}
	}
}

void testing()
{
	using namespace std;
	
	Player p1  (Piece::BLACK);
	p1.display();
	cout << endl <<endl;
	Player p2 (Piece::RED);
	p2.display();
	p1.printgame();
	p1.printgame();
	p1.movePiece(9, Player::LEFT);
	p1.movePiece(8, Player::LEFT);
	p1.movePiece(8, Player::RIGHT);
	p2.movePiece(10,Player::LEFT);
	p1.printgame();
}

		

int main()
{
	dostuff();
	return 0;
}
