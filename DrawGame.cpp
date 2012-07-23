/*
 * DrawGame.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#include <allegro.h>
#include <iostream>

#include "bst.hpp"
#include "checkers.hpp"
#include "player.hpp"
#include "game.hpp"
#include "DrawGame.hpp"

BITMAP *buffer;

DrawGame::DrawGame()
: Game(true,true)
{
	const unsigned DRAW_SIZE = BOARD_SIZE * 30;
	allegro_init();
	install_keyboard();
	set_gfx_mode( GFX_AUTODETECT_WINDOWED, DRAW_SIZE, DRAW_SIZE, 0, 0);
	buffer = create_bitmap(DRAW_SIZE,DRAW_SIZE);
	_print();
}

DrawGame::DrawGame(const SaveGame& record)
: Game(record,true,true)
{
	const unsigned DRAW_SIZE = BOARD_SIZE * CELL_SIZE;
	allegro_init();
	install_keyboard();
	set_gfx_mode( GFX_AUTODETECT_WINDOWED, DRAW_SIZE, DRAW_SIZE, 0, 0);
	buffer = create_bitmap(DRAW_SIZE,DRAW_SIZE);
	_print();
}

void DrawGame::_print() const
{
	const int TAN = makecol(0xff,0xda,0xb9);
	const int BLACK = makecol(0x0,0x0,0x0);
	const int RED = makecol(0xff,0x0,0x0);
	const int SADDLEBROWN = makecol(0x8b,0x45,0x13);

	clear_to_color(buffer,TAN);
	/* Go through board, filling in cells according
	 * to status
	 */
	unsigned xcord = 0, ycord = 0;
	for (unsigned i = 0; i < BOARD_SIZE; i++) {
		ycord = 0;
		for (int j = BOARD_SIZE - 1; j >= 0; j--) {
			if (!((i+j) % 2)) {
				rectfill(buffer,xcord,ycord,xcord+CELL_SIZE, ycord+CELL_SIZE,SADDLEBROWN);
			}
			if ((*this)[i][j].getInPlay() ) {
				int pieceCol;
				if ( (*this)[i][j].getColor() == Piece::RED) {
					pieceCol = RED;
				} else {
					pieceCol = BLACK;
				}
				circlefill(buffer,xcord + CELL_SIZE/2,ycord + CELL_SIZE/2, CELL_SIZE/2,pieceCol);
			}
			ycord += CELL_SIZE;
		}
		xcord += CELL_SIZE;
	}
	blit(buffer,screen,0,0,0,0,BOARD_SIZE * CELL_SIZE,BOARD_SIZE * CELL_SIZE);
}

DrawGame::~DrawGame()
{
	readkey();
	destroy_bitmap(buffer);
	allegro_exit();
}
END_OF_MAIN()

//int DrawGame::_receiveInput()
//{
//	return super::receiveInput();
//}

//int main()
//{
//	DrawGame *theMatch = new DrawGame();
//	playAIvsAI(theMatch, true);
//}

int main() {
	using namespace std;

	string instr;
	DrawGame *theMatch;
	SaveGame *loadGame, *saveGame;
	cout << "Would you like to load a game? (y/n): ";
	getline(cin, instr);
	if (instr == "y") {
		cout << "Enter save file path: ";
		getline(cin, instr);
		try {
			loadGame = new SaveGame(false);
			loadGame->read(instr);
		} catch (const ios_base::failure&) {
			cerr << "Error loading savefile \"" << instr << "\"" << endl;
			return 1;
		}
		theMatch = new DrawGame(*loadGame);
		delete loadGame;
	} else {
		theMatch = new DrawGame();
	}
	cout << "Please choose an option by entering it's number :\n"
			<< "1.    Play against AI\t\t2.    Play against a friend\n"
			<< "3.    Watch 2 AI's play each other.\n\n";
	getline(cin, instr);

	if (instr == "1")
		playAgainstAI(theMatch, true);
	else if (instr == "2") {
		playPvP(theMatch);
	} else if (instr == "3") {
		playAIvsAI(theMatch, true);
	} else {
		cerr << "I'm sorry you haven't chosen a valid option. Goodbye!\n";
		return 1;
	}

	cout << "Would you like to save your game? (y/n): ";
	getline(cin, instr);
	if (instr == "y") {
		saveGame = new SaveGame(theMatch->getSave());
		cout << "Enter save file path: ";
		getline(cin, instr);
		saveGame->write(instr);
		delete saveGame;
	}
	delete theMatch;
	return 0;
}
