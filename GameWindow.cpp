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
				m_filename_entry(),
				m_file_entry_label("Enter file name: ")
{
	set_has_resize_grip (false);
	set_resizable(false);

	Gtk::Box* area = get_content_area();
	area->add(m_file_entry_label);
	m_filename_entry.set_activates_default(true);
	area->add(m_filename_entry);
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
	return m_filename_entry.get_text();
}

void SaveDialog::entry(const std::string& s) {
	m_filename_entry.set_text(s);
}

void setStock(Gtk::Button& button, std::string label, const Gtk::StockID stock)
{
	Gtk::Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_BUTTON));
	button.set_image(*img);
	button.set_label(label);
}

GameWindow::GameWindow() :
				m_toplevel_hbox(Gtk::ORIENTATION_HORIZONTAL),
				m_grid(),
				m_game_widget(sf::VideoMode(800, 800, 32)),
				m_file_box(Gtk::ORIENTATION_HORIZONTAL),
				m_save_game_button("Save"),
				m_load_game_button("Load"),
				m_checkpoint_box(),
				m_checkpoint_button(),
				m_checkpoint_restore_button(),
				m_button_quit("Quit"),
				m_checkpoint()
{
	set_title("Custom Widget example");
	set_border_width(6);
	set_default_size(900, 800);

	add(m_toplevel_hbox);
	m_toplevel_hbox.pack_start(m_grid, Gtk::PACK_EXPAND_PADDING);

	m_grid.set_row_homogeneous(true);
	m_grid.attach(m_file_box, 0, 0, 1, 1);

	m_file_box.pack_start(m_save_game_button, Gtk::PACK_EXPAND_PADDING);
	m_file_box.pack_start(m_load_game_button, Gtk::PACK_EXPAND_PADDING);
	m_file_box.set_border_width(6);
	m_file_box.set_layout(Gtk::BUTTONBOX_END);

	setStock(m_save_game_button, "Save Game", Gtk::Stock::FILE);
	m_save_game_button.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onSaveGameClick));

	setStock(m_load_game_button, "Load Game", Gtk::Stock::FILE);
	m_load_game_button.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onLoadGameClick));

	m_grid.attach(m_button_quit, 0, 4, 1, 1);
	setStock(m_button_quit, "Quit", Gtk::Stock::QUIT);
	m_button_quit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onQuitClick));

	m_toplevel_hbox.pack_start(m_game_widget, Gtk::PACK_EXPAND_PADDING);
	m_game_widget.set_hexpand(false);
	m_game_widget.set_vexpand(false);
	m_game_widget.show();
	m_game_widget.set_size_request(800, 800);

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
		m_game_widget._game.restoreToSave(s);
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
		m_game_widget._game.getSave().write("saves/" + filename + ".cks");
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
