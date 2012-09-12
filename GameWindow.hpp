/*
 * GameWindow.hpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#ifndef GAMEWINDOW_HPP_
#define GAMEWINDOW_HPP_

#include "SFMLWidget.hpp"
#include <gtkmm.h>


class LoadGameDialog : public Gtk::Dialog {
public:
	LoadGameDialog();
	virtual ~LoadGameDialog();

protected:
	Gtk::Button* yesButton;
	Gtk::Button* noButton;

	void onYesButtonClicked();
	void onNoButtonClicked();
};

class GameWindow: public Gtk::Window {
public:
	GameWindow();
	virtual ~GameWindow();

protected:
	//Signal handlers:
	void onQuitClick();

	void onSaveGameClick();

	void onLoadGameClick();

	void onCanvasClick(Gdk::Event* e);

	// Layout containers
	Gtk::Box _hBox; // TopLevel
	Gtk::Grid _grid0;

	//Child widgets:
	SFMLWidget _gameWidget;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button _buttonSaveGame;
	Gtk::Button _buttonLoadGame;
	Gtk::Button _buttonQuit;
};

#endif /* GAMEWINDOW_HPP_ */
