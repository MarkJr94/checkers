#include <iostream>
#include <string>

#include "checkers.hpp"
#include "player.hpp"
int  receiveInput(unsigned& piece, Direction& d)
{
	using namespace std;
	string dirString;
	
	cout << "Enter piece id: ";
	cin  >> piece;
	cout << "Enter Direction 'l' = left or 'r' = right: ";
	cin >> dirString;
	if  (dirString == "left")
		d = LEFT;
	else if (dirString == "right")
		d = RIGHT;
	else
		return 0;
	
	return 1;
}
	
void dostuff() {
	using namespace std;
	
	Player player1 (BLACK);
	Player player2 (RED);
	player1.display(); player2.display();
	player1.printgame();
	unsigned piece;
	Direction d;
	
	while (1) {
		while (1) {
			cout << "================ Player 1 (Black) ==============\n\n";
			if (!receiveInput(piece,d)) {
				cerr << "input error, try again\n";
				continue;
			}
			if (!player1.movePiece(piece,d)) {
				cerr << "movement error, try again\n";
				continue;
			}
			break;
		}
		player1.printgame();
		while (1) {
			cout << "================ Player 2  (Red) ==============\n\n";
			if (!receiveInput(piece,d)) {
				cerr << "input error, try again\n";
				continue;
			}
			if (!player2.movePiece(piece,d)) {
				cerr << "movement error, try again\n";
				continue;
			}
			break;
		}
		player2.printgame();
	}
}

void testing()
{
	using namespace std;
	
	Player p1  (BLACK);
	p1.display();
	cout << endl <<endl;
	Player p2 (RED);
	p2.display();
	Piece::printBoard();
	p1.printgame();
	p1.printgame();
	p1.movePiece(9, LEFT);
	p1.movePiece(8, LEFT);
	p1.movePiece(8, RIGHT);
	p2.movePiece(10,LEFT);
	//~ Piece::printBoard();
	p1.printgame();
	//~ p1.display();
}

		

int main()
{
	dostuff();
	return 0;
}
