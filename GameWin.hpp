/*
 * DrawGame.h
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#pragma once
#ifndef DRAWGAME_HPP_
#define DRAWGAME_HPP_

#include "BitBoard.hpp"
#include "Save.hpp"
#include "Game.hpp"
#include "AI.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

const int circRad = 40;
const int cellSz = 100;

class BaseWin : public sf::RenderWindow {
public:
	BaseWin(const sf::VideoMode& m, const std::string& title) : sf::RenderWindow(m,title) {}
	virtual ~BaseWin() {}

	virtual void draw() = 0;
};

class GameWin : public BaseWin {
public:
	GameWin(const int wide = 800, const int high = 800);
	~GameWin();
	void draw();
	void sfHandleEvents();

	void bindGame(Game * game = new Game(false,false)) {
		delete _game;
		_game = game;
	}

	void drawCell(Cell c,int x, int y);

	void loop();

	enum State {NORMAL, HLIGHT_1, EVALUATING};

private:
	typedef BaseWin super;
	typedef std::vector<Cell> Board;
	Game* _game;
	AI _ai;
	Board _board;
	sf::Vector2<int> _mDown1;
	sf::Vector2<int> _mDown2;
	State _state;

	sf::Vector2<int> resolveMouse(sf::Vector2<float>) const;
	MoveCode evalSelections();
};

#endif /* GAME_HPP_ */
