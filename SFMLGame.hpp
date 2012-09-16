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


// SFML Includes
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Gtk Includes
#include <gtkmm.h>

// GLib Includes
#include <glibmm.h>

// GDK Includes
#include <gdkmm.h>
#include <gdk/gdkx.h>

class SFMLGame : public sf::RenderWindow, public Gtk::Widget {
public:


	friend class GameWindow;
	friend class SFMLWidget;

	SFMLGame(const int wide = 800, const int high = 800);
	SFMLGame(const sf::VideoMode& mode = sf::VideoMode(800,800,32));
	virtual ~SFMLGame();

	/* ===================== SFML Interface ============= */
	void update();

	void drawCell(Cell c,int x, int y);

	void loop();

	enum State {NORMAL, HLIGHT_1, EVALUATING};

	/* ===================== Gtkmm Interface =================== */

private:
	/* ===================== SFML Implementation ============= */
	typedef sf::RenderWindow super;
	typedef std::vector<Cell> Board;
	Game _game;
	AI _ai;
	Board _board;
	sf::Vector2<int> _mDown1;
	sf::Vector2<int> _mDown2;
	State _state;

	int circRad = 40;
	int cellSz = 100;

	sf::Vector2<int> resolveMouse(sf::Vector2<int>) const;

	MoveCode evalSelections();

	/* ===================== Gtkmm Implementation =================== */

	virtual void on_size_request(Gtk::Requisition* requisition);

	virtual void on_size_allocate(Gtk::Allocation& allocation);

	virtual void on_map();

	virtual void on_unmap();

	virtual void on_realize();

	virtual void on_unrealize();

	virtual bool on_idle();

	virtual bool on_expose_event(GdkEventExpose* event);

	virtual bool on_button_press_event(GdkEventButton* event);

	virtual bool on_key_press_event(GdkEventKey* event);

	Glib::RefPtr<Gdk::Window> _gdkWindow;
};

#endif /* GAME_HPP_ */
