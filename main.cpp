#include <iostream>
#include <string>

#include "checkers.hpp"
#include "player.hpp"

class Game {
	Player p1;
	Player p2;
	bool turn;
	bool debug;
	
public:
	Game(bool db = true) 
			: p1 (Piece::BLACK,db), p2 (Piece::RED,db), turn (true), debug (db)
	{;}
	
	int receiveInput(unsigned piece, Player::Direction d)
	{
		using namespace std;
		string dirString;
		unsigned prey;
		Player *a, *b;
		if (turn) {
			a = &p1;
			b = &p2;
		} else {
			a = &p2;
			b = &p1;
		}
		Player& player = *a;
		Player& other = *b;
		
		if (debug) cout << "Enter piece id: ";
		cin  >> piece;
		if (debug) cout << "('q' = quit)\tEnter Direction 'l' = left or 'r' = right or 'j' = jump: ";
		cin >> dirString;
		if (dirString == "q") return -1;
		/* Jumping */
		if (dirString == "j") {
			if (debug) cout << "Enter prey ID: ";
			cin >> prey;
			if (!player.jumpPiece(piece,prey,other)) {
				cerr << "Jumping error; try again\n";
				return 0;
			}
			turn = !turn;
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
		turn = !turn;
		return 1;
	}
	
	void play()
	{
		using namespace std;
		
		if (debug) p1.display();
		if (debug) p2.display();
		if (debug) p1.printgame();
		
		unsigned count1 = 0,count2 = 0;
		unsigned piece;
		Player::Direction d;
		
		while (1) {
			if (debug) cout << p1.getnPieces() << " Player 1\n" << p2.getnPieces() << " Player 2\n\n";
			if (p1.getnPieces() < 1) return;
			if (debug) cout << "P1 pieces = " << p1.getnPieces() << endl;
			while (1) {
				if (count1 == 3) {
					count1 = 0;
					if (debug) cout << "Three failed moves P1; Lose a turn!\n\n";
					break;
				}
				if (debug) cout << "================ Player 1 (Black) ==============\n\n";
				int c = receiveInput(piece,d);
				switch (c) {
					case -1: return;
					case 0: {
						count1++;
						continue;
					}
					default:;
				}
				count1 = 0;
				if (debug) p1.printgame();
				break;
			}
			if (p2.getnPieces() <1) return;
			if (debug) cout << "P2 pieces = " << p2.getnPieces() << endl;
			while (1) {
				if (count2 == 3) {
					count2 = 0;
					if (debug) cout << "Three failed moves P2; Lose a turn!\n\n";
					break;
				}
				if (debug) cout << "================ Player 2  (Red) ==============\n\n";
				int c = receiveInput(piece,d);
				switch (c) {
					case -1: return;
					case 0: {
						count2++;
						continue;
					}
					default:;
				}
				count2 = 0;
				if (debug) p2.printgame();
				break;
			}
		}
	}
};

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
	Game checkers (true);
	checkers.play();
	return 0;
}
