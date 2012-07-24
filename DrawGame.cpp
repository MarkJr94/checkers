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


