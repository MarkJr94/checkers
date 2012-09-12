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

Gtk::Button* set_but(const char *label, const Gtk::StockID& stock)
{
	using namespace Gtk;
	Button *button = manage(new Button(label));
	Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_BUTTON));
	button->set_image(*img);
	button->set_label(label);

	return button;
}

void setStock(Gtk::Button& button, std::string label, const Gtk::StockID stock) {
	Gtk::Image *img = manage(new Gtk::Image(stock, Gtk::ICON_SIZE_BUTTON));
	button.set_image(*img);
	button.set_label(label);
}

LoadGameDialog::LoadGameDialog()
{
	using namespace Gtk;

	ButtonBox* actionArea = get_action_area();
	yesButton = set_but("Yes", Gtk::Stock::DIRECTORY);
	yesButton->signal_clicked().connect(
			sigc::mem_fun(*this, &LoadGameDialog::onYesButtonClicked));
	noButton = set_but("No", Stock::NO);
	noButton->signal_clicked().connect(
			sigc::mem_fun(*this, &LoadGameDialog::onNoButtonClicked));

	actionArea->pack_start(*noButton, false, false);
	actionArea->pack_start(*yesButton, false, false);

	show_all_children();
}

LoadGameDialog::~LoadGameDialog()
{

}

void LoadGameDialog::onYesButtonClicked()
{

}

void LoadGameDialog::onNoButtonClicked()
{
	hide();
}

GameWindow::GameWindow() :
				_hBox(Gtk::ORIENTATION_HORIZONTAL),
				_grid0(),
				_gameWidget(sf::VideoMode(800, 800, 32)),
				m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL),
				_buttonSaveGame("Save"),
				_buttonLoadGame("Load"),
				_buttonQuit("Quit")
{
	set_title("Custom Widget example");
	set_border_width(6);
	set_default_size(900, 800);

	add(_hBox);
	_hBox.pack_start(_grid0, Gtk::PACK_EXPAND_PADDING);

	_grid0.set_row_homogeneous(true);
	_grid0.attach(m_ButtonBox,0,0,1,1);
	_grid0.set_hexpand(false);
//	_hBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	m_ButtonBox.pack_start(_buttonSaveGame, Gtk::PACK_EXPAND_PADDING);
	m_ButtonBox.pack_start(_buttonLoadGame,Gtk::PACK_EXPAND_PADDING);
	m_ButtonBox.set_border_width(6);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

	setStock(_buttonSaveGame,"Save Game",Gtk::Stock::FILE);
	_buttonSaveGame.signal_clicked().connect(
				sigc::mem_fun(*this, &GameWindow::onSaveGameClick));

	setStock(_buttonLoadGame,"Load Game", Gtk::Stock::FILE);
	_buttonLoadGame.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onLoadGameClick));

	_grid0.attach(_buttonQuit,0,4,1,1);
	setStock(_buttonQuit,"Quit",Gtk::Stock::QUIT);
	_buttonQuit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::onQuitClick));

	SFMLGame* msd = new SFMLGame(800, 800);
	msd->bindGame(new Game(false, false));
	_gameWidget.bindWin(msd);
//	_gameWidget.signal_clicked().connect(
//			sigc::mem_fun(*this, &GameWindow::onCanvasClick));


	_hBox.pack_start(_gameWidget, Gtk::PACK_EXPAND_PADDING);
	_gameWidget.set_hexpand(false);
	_gameWidget.set_vexpand(false);
	_gameWidget.show();
	_gameWidget.set_size_request(800,800);

	show_all_children();
}

GameWindow::~GameWindow()
{
}

void GameWindow::onQuitClick()
{
	hide();
}

void GameWindow::onCanvasClick(Gdk::Event* e) {
	double x, y;
	e->get_coords(x,y);
	std::cout << "x: " << x << " y: " << y << std::endl;
}

void GameWindow::onLoadGameClick()
{
  Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

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
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      std::cout << "Open clicked." << std::endl;

      //Notice that this is a std::string, not a Glib::ustring.
      std::string filename = dialog.get_filename();
      std::cout << "File selected: " <<  filename << std::endl;

      Save s;
      s.read(filename);
      _gameWidget._sfRenWin->_game->restoreToSave(s);
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}

void GameWindow::onSaveGameClick()
{
  Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);

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
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      std::cout << "Open clicked." << std::endl;

      //Notice that this is a std::string, not a Glib::ustring.
      std::string filename = dialog.get_filename();
      std::cout << "File selected: " <<  filename << std::endl;
      _gameWidget._sfRenWin->_game->getSave().write(filename + ".cks");
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}
