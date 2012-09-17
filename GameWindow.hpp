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
	Gtk::Entry m_filename_entry;
	Gtk::Label m_file_entry_label;
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
	Gtk::Box m_toplevel_hbox; // TopLevel
	Gtk::Grid m_grid;

	//Child widgets:
	SFMLGame m_game_widget;

	Gtk::ButtonBox m_file_box;
	Gtk::Button m_save_game_button;
	Gtk::Button m_load_game_button;

	Gtk::ButtonBox m_checkpoint_box;
	Gtk::Button m_checkpoint_button;
	Gtk::Button m_checkpoint_restore_button;

	Gtk::Button m_button_quit;

	Save m_checkpoint;
};

#endif /* GAMEWINDOW_HPP_ */
