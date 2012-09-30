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
	Gtk::Entry mEntryFilename;
	Gtk::Label mLabelFilename;
	typedef Gtk::Dialog super;
};

class GameWindow: public Gtk::Window {
public:
	GameWindow();
	virtual ~GameWindow();

protected:
	//Signal handlers:
	void onQuitClick();

	void onSaveQuitClick();

	void onSaveClick();

	void onLoadClick();

	void onCheckpointClick();

	void onRestoreClick();

	// Layout containers
	Gtk::HBox mHBox;
	Gtk::VBox mVBox; // TopLevel
//	Gtk::Grid mGrid;

	//Child widgets:
	SFMLGame mGameWidget;

	Gtk::ButtonBox mBoxFile;
	Gtk::Button mButtonSave;
	Gtk::Button mButtonLoad;

	Gtk::ButtonBox mBoxCheckpoint;
	Gtk::Button mButtonCheckpoint;
	Gtk::Button mButtonRestore;

	Gtk::Box mBoxQuit;
	Gtk::Button mButtonSaveQuit;
	Gtk::Button mButtonQuit;

	Save mCheckpoint;

	typedef Gtk::Window super;
};

#endif /* GAMEWINDOW_HPP_ */
