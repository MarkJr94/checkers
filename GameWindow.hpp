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

class SaveDialog: public Gtk::Dialog {
public:
	SaveDialog(const std::string& title, bool modal = true);
	~SaveDialog();
	std::string entry();
	void entry(const std::string&);

private:
	Gtk::Entry _fileEnt;
	Gtk::Label _label;
	typedef Gtk::Dialog super;
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

	// Layout containers
	Gtk::Box _hBox; // TopLevel
	Gtk::Grid _grid0;

	//Child widgets:
	SFMLGame _gameWidget;

	Gtk::ButtonBox m_file_box;
	Gtk::Button _mButtonSaveGame;
	Gtk::Button _buttonLoadGame;

	Gtk::ButtonBox _checkBox;
	Gtk::Button _buttonCheckpoint;
	Gtk::Button _buttonRestoreCheckpoint;

	Gtk::Button _buttonQuit;

	Save
};

#endif /* GAMEWINDOW_HPP_ */
