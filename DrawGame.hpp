/*
 * DrawGame.h
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#pragma once
#ifndef DRAWGAME_HPP_
#define DRAWGAME_HPP_

#include "Piece.hpp"
#include "Save.hpp"
#include "Game.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



class DrawGame : public Game {
public:
	friend class GameWin;

	/* Constructor */
	DrawGame(bool db = false, bool interact = false);
	/* Constructor from memory */
	DrawGame(const Save& record, bool db = false, bool interact = false);
	/* Destructor */
	~DrawGame();

	enum State { MOVING, READY };

private:
	typedef Game super;
};

class GameWin : public sf::RenderWindow {
public:
	GameWin(const int wide = 800, const int high = 640);
	~GameWin();
	void drawGame();
	bool sfHandleEvents();
	void createGame(bool db = false, bool interact = false) {
		game = new DrawGame(db,interact);
	}
	/* Constructor from memory */
	void createGame(const Save& record, bool db = false, bool interact = false) {
		game = new DrawGame(record,db,interact);
	}

private:
	typedef sf::RenderWindow super;
	typedef std::vector<std::vector<Piece> > GameBoard;
	DrawGame* game;
};

#endif /* GAME_HPP_ */
