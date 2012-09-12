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
				m_MyWidget(sf::VideoMode(800, 800, 32)),
				m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL),
				m_Button_Quit("Quit")
{
	set_title("Custom Widget example");
	set_border_width(6);
	set_default_size(900, 800);

	add(_hBox);
	_hBox.pack_start(_grid0, Gtk::PACK_SHRINK);

	_grid0.attach(m_ButtonBox,0,0,1,1);
	_grid0.set_hexpand(true);
//	_hBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
	m_ButtonBox.set_border_width(6);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
	m_Button_Quit.signal_clicked().connect(
			sigc::mem_fun(*this, &GameWindow::on_button_quit));

	SFMLGame* msd = new SFMLGame(800, 800);
	msd->bindGame(new Game(false, false));
	m_MyWidget.bindWin(msd);
	_hBox.pack_start(m_MyWidget, Gtk::PACK_EXPAND_WIDGET);
	m_MyWidget.set_hexpand(false);
	m_MyWidget.set_vexpand(false);
	m_MyWidget.show();
	m_MyWidget.set_size_request(800,800);

	show_all_children();
}

GameWindow::~GameWindow()
{
}

void GameWindow::on_button_quit()
{
	hide();
}
