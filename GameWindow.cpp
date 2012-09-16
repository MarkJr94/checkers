/*
 * GameWindow.cpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#include <iostream>
#include <glibmm.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameWindow.hpp"

SaveDialog::SaveDialog(const std::string& title, bool modal) :
				super(title, modal),
				_fileEnt(),
				_label("Enter file name: ")
{
	set_has_resize_grip (false);
	set_resizable(false);

	Gtk::Box* area = get_content_area();
	area->add(_label);
	_fileEnt.set_activates_default(true);
	area->add(_fileEnt);
//	area->pack_start(_label,true,true);
//	area->pack_end(_fileEnt,true,true);

	Gtk::Button* ok =  (manage(new Gtk::Button (Gtk::Stock::OK)));
	ok->set_can_default(true);
	set_default(*ok);
	add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	add_action_widget(*ok,Gtk::RESPONSE_OK);

	show_all_children();
}

SaveDialog::~SaveDialog() {}

std::string SaveDialog::entry() {
	return _fileEnt.get_text();
}

void SaveDialog::entry(const std::string& s) {
	_fileEnt.set_text(s);
}

void setStock(Gtk::Button& button, std::string label, const Gtk::StockID stock)
{
	Gtk::Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_BUTTON));
	button.set_image(*img);
	button.set_label(label);
}

GameWindow::GameWindow() :
				_hBox(Gtk::ORIENTATION_HORIZONTAL),
				_grid0(),
				_gameWidget(sf::VideoMode(800, 800, 32)),
				m_file_box(Gtk::ORIENTATION_HORIZONTAL),
				_mButtonSaveGame("Save"),
				_buttonLoadGame("Load"),
				_buttonQuit("Quit")
{
	set_title("Custom Widget example");
	set_border_width(6);
	set_default_size(900, 800);

	add(_hBox);
	_hBox.pack_start(_grid0, Gtk::PACK_EXPAND_PADDING);

	_grid0.set_row_homogeneous(true);
	_grid0.attach(m_file_box, 0, 0, 1, 1);
	_grid0.set_hexpand(false);
//	_hBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	m_file_box.pack_start(_mButtonSaveGame, Gtk::PACK_EXPAND_PADDING);
	m_file_box.pack_start(_buttonLoadGame, Gtk::PACK_EXPAND_PADDING);
	m_file_box.set_border_width(6);
	m_file_box.set_layout(Gtk::BUTTONBOX_END);

	setStock(_mButtonSaveGame, "Save Game", Gtk::Stock::FILE);
	_mButtonSaveGame.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onSaveGameClick));

	setStock(_buttonLoadGame, "Load Game", Gtk::Stock::FILE);
	_buttonLoadGame.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onLoadGameClick));

	_grid0.attach(_buttonQuit, 0, 4, 1, 1);
	setStock(_buttonQuit, "Quit", Gtk::Stock::QUIT);
	_buttonQuit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onQuitClick));

	_hBox.pack_start(_gameWidget, Gtk::PACK_EXPAND_PADDING);
	_gameWidget.set_hexpand(false);
	_gameWidget.set_vexpand(false);
	_gameWidget.show();
	_gameWidget.set_size_request(800, 800);

	show_all_children();
}

GameWindow::~GameWindow()
{
}

void GameWindow::onQuitClick()
{
	hide();
}

void GameWindow::onLoadGameClick()
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
		_gameWidget._game.restoreToSave(s);
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

void GameWindow::onSaveGameClick() {
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
		_gameWidget._game.getSave().write("saves/" + filename + ".cks");
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
