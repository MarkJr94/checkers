/*
 * DrawGame.cpp
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#include <iostream>
#include <vector>
#include <sstream>

#include "DrawGame.hpp"

DrawGame::DrawGame(bool db, bool interact) :
		super(db, interact) {
}

DrawGame::DrawGame(const Save& record, bool db, bool interact) :
		super(record,db,interact) {
}

DrawGame::~DrawGame() {
}


GameWin::GameWin(const int wide, const int high) :
		super(sf::VideoMode(800, 600, 32), "Game Window") {

}

GameWin::~GameWin() {
//	delete game;
}

bool GameWin::sfHandleEvents() {
	sf::Event Event;
	bool ret = false;
	while (GetEvent(Event)) {
		// Window closed
		if (Event.Type == sf::Event::Closed) {
			Close();
			ret = true;
		}
		// Escape key pressed
		if ((Event.Type == sf::Event::KeyPressed)
				&& (Event.Key.Code == sf::Key::Escape)) {
			Close();
			ret = true;
		}
	}
	return ret;
}

void GameWin::drawGame() {
	using namespace std;
	using namespace sf;

	GameBoard& board = game->board;

	GameBoard::const_reverse_iterator rit = board.rbegin();

	while (rit != board.rend()) {
		for (auto& piece : *rit) {
			Shape cell = Shape::Circle(piece.x+25,piece.y+25,40,Color(0xff,0,0));
			Draw(cell);
		}
	}
}
