/*
 * DrawGame.h
 *
 *  Created on: Jul 22, 2012
 *      Author: markjr
 */

#pragma once
#ifndef DRAWGAME_HPP_
#define DRAWGAME_HPP_

#include "checkers.hpp"
#include "Save.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



class DrawGame {
public:
	friend class GameWin;

	/* Constructor */
	DrawGame(bool db = false, bool interact = false);
	/* Constructor from memory */
	DrawGame(const Save& record, bool db = false, bool interact = false);
	/* Destructor */
	~DrawGame();

	/* Update save game */
	inline void updateSave();
	/* return save game */
	Save getSave();
	/* Print game */
	void print() const;
	/* Draw */
	void draw(double dt);

	/* Enumerations for movement and jump directions */
	enum Direction {
		LEFT, RIGHT, BKLEFT, BKRIGHT
	};

	enum State { MOVING, READY };

	/* Piece Movement */
	bool movePiece(unsigned piece, Direction d);
	/* Jumping */
	bool jumpPiece(unsigned jumper, unsigned prey);
	/* restore game to save */
	void restoreToSave(const Save& record);
	/* Receive input for CLI */
	int receiveInput();

	int getMustJump() const {
		return mustJump;
	}

	void setMustJump(int mustJump) {
		this->mustJump = mustJump;
	}
	/* Get p1 score */
	inline unsigned getP1score() const {
		return p1.size();
	}
	/* Get p2 score */
	inline unsigned getP2score() const {
		return p2.size();
	}
	/* Setter and getter for turn */
	void setTurn(bool newval) {
		turn = newval;
	}
	bool getTurn() const {
		return turn;
	}
	std::vector<Piece>& operator[](unsigned row) {
		return board[row];
	}
	const std::vector<Piece>& operator[](unsigned row) const {
		return board[row];
	}

private:
	std::map <int,Piece *> p1;
	std::map <int,Piece *> p2;
	std::vector<std::vector<Piece> > board;
	/* Tracks if it's P1's turn or not */
	bool turn;
	bool debug;
	Save save;
	bool interact;
	unsigned mustJump;

	/* Save of initial game for simpler construction */
	static Save templateSave;
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
