/*
 * GameWindow.cpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#include <iostream>
#include <sstream>

#include <glibmm.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameWindow.hpp"

template <typename T>
std::string tostr(const T& t)
{
	std::stringstream ss;
	return (ss << t).str();
}

template <typename T>
std::string& operator<<(std::string& str, const T& t)
{
	std::stringstream ss;
	return (ss << t).str();
}

SaveDialog::SaveDialog(const std::string& title, bool modal) :
				super(title, modal),
				mEntryFilename(),
				mLabelFilename("Enter file name: ")
{
	set_has_resize_grip(false);
	set_resizable(false);

	Gtk::Box* area = get_content_area();
	area->add(mLabelFilename);
	mEntryFilename.set_activates_default(true);
	area->add(mEntryFilename);
//	area->pack_start(_label,true,true);
//	area->pack_end(_fileEnt,true,true);

	Gtk::Button* ok = (manage(new Gtk::Button(Gtk::Stock::OK)));
	ok->set_can_default(true);
	set_default(*ok);
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_action_widget(*ok, Gtk::RESPONSE_OK);

	show_all_children();
}

SaveDialog::~SaveDialog()
{
}

std::string SaveDialog::entry()
{
	return mEntryFilename.get_text();
}

void SaveDialog::entry(const std::string& s)
{
	mEntryFilename.set_text(s);
}

void setStock(Gtk::Button& button, std::string label, const Gtk::StockID stock)
{
	Gtk::Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_DIALOG));
	button.set_image(*img);
	button.set_label(label);
}

GameWindow::GameWindow() :
				super(),
				mVBox(Gtk::ORIENTATION_HORIZONTAL),
//				mGrid(),
				mGameWidget(sf::VideoMode(800, 800, 32)),
				mBoxFile(Gtk::ORIENTATION_HORIZONTAL),
				mButtonSave(),
				mButtonLoad(),
				mBoxCheckpoint(),
				mButtonCheckpoint(),
				mButtonRestore(),
				mBoxQuit(),
				mButtonSaveQuit("Save/Exit"),
				mButtonQuit("Quit"),
				mCheckpoint()
{
	set_title("Simple Checkers");
	set_border_width(6);
	set_default_size(800, 800);
	set_resizable(false);

	add(mHBox);

	mHBox.pack_start(mVBox,Gtk::PACK_EXPAND_WIDGET);
	mVBox.set_homogeneous(true);

	mVBox.pack_start(mButtonSave,Gtk::PACK_EXPAND_WIDGET);
	mVBox.pack_start(mButtonLoad,Gtk::PACK_EXPAND_WIDGET);

	setStock(mButtonSave, "Save Game", Gtk::Stock::SAVE);
	mButtonSave.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onSaveClick));

	setStock(mButtonLoad, "Load Game", Gtk::Stock::DIRECTORY);
	mButtonLoad.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onLoadClick));

	mVBox.pack_start(mButtonCheckpoint, Gtk::PACK_EXPAND_WIDGET);
	mVBox.pack_start(mButtonRestore, Gtk::PACK_EXPAND_WIDGET);

	setStock(mButtonCheckpoint, "Checkpoint", Gtk::Stock::APPLY);
	mButtonCheckpoint.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onCheckpointClick));

	setStock(mButtonRestore, "Restore", Gtk::Stock::REVERT_TO_SAVED);
	mButtonRestore.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onRestoreClick));

	mVBox.pack_start(mButtonSaveQuit, Gtk::PACK_EXPAND_WIDGET);
	mVBox.pack_start(mButtonQuit, Gtk::PACK_EXPAND_WIDGET);

//	setStock(m_button_save_quit,"Save and Exit",Gtk::Stock::DIRECTORY);
	mButtonSaveQuit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onQuitClick));

	setStock(mButtonQuit, "Quit", Gtk::Stock::QUIT);
	mButtonQuit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onQuitClick));

	mHBox.pack_start(mGameWidget, Gtk::PACK_EXPAND_PADDING);

	mGameWidget.set_hexpand(false);
	mGameWidget.set_vexpand(false);
	mGameWidget.show();
	mGameWidget.set_size_request(800, 800);

	show_all_children();
}

GameWindow::~GameWindow()
{
}

void GameWindow::onQuitClick()
{
	hide();
}

void GameWindow::onLoadClick()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.set_current_folder("saves");

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:

	Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Checkers Save File");
	filter_any->add_pattern("*.cks");
	dialog.add_filter(filter_any);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	//Handle the response:
	switch (result) {
	case (Gtk::RESPONSE_OK): {
		std::cout << "Open clicked." << std::endl;

		//Notice that this is a std::string, not a Glib::ustring.
		std::string filename = dialog.get_filename();
		std::cout << "File selected: " << filename << std::endl;

		Save s;
		s.read(filename);
		mGameWidget.mGame.restoreToSave(s);
		break;
	}
	case (Gtk::RESPONSE_CANCEL): {
		std::cout << "Cancel clicked." << std::endl;
		break;
	}
	default: {
		std::cout << "Unexpected button clicked." << std::endl;
		break;
	}
	}
}

void GameWindow::onSaveClick()
{
	SaveDialog dialog("Save your game", true);

	dialog.set_transient_for(*this);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	//Handle the response:
	switch (result) {
	case (Gtk::RESPONSE_OK): {
		std::cout << "Open clicked." << std::endl;

		//Notice that this is a std::string, not a Glib::ustring.
		std::string filename = dialog.entry();
		std::cout << "File selected: " << filename << std::endl;
		mGameWidget.mGame.getSave().write("saves/" + filename + ".cks");
		break;
	}
	case (Gtk::RESPONSE_CANCEL): {
		std::cout << "Cancel clicked." << std::endl;
		break;
	}
	default: {
		std::cout << "Unexpected button clicked." << std::endl;
		break;
	}
	}
}

void GameWindow::onCheckpointClick()
{
	mCheckpoint = mGameWidget.mGame.getSave();
}

void GameWindow::onRestoreClick()
{
	mGameWidget.mGame.restoreToSave(mCheckpoint);
}

void GameWindow::onSaveQuitClick()
{
	onSaveClick();
	hide();
}
