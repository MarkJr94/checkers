/*
 * GameWindow.hpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#ifndef GAMEWINDOW_HPP_
#define GAMEWINDOW_HPP_

#include "SFMLGame.hpp"
#include <gtkmm.h>

class GameWindow: public Gtk::Window {
public:
	GameWindow();
	virtual ~GameWindow();

protected:
	//Signal handlers:
	void onQuitClick();

	void onSaveGameClick();

	void onLoadGameClick();

	// Layout containers
	Gtk::Box _hBox; // TopLevel
	Gtk::Grid _grid0;

	//Child widgets:
	SFMLGame _gameWidget;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button _buttonSaveGame;
	Gtk::Button _buttonLoadGame;
	Gtk::Button _buttonQuit;
};

#endif /* GAMEWINDOW_HPP_ */
