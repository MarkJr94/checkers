/*
 * GameWindow.cpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#include <iostream>
#include <glibmm/refptr.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/stockid.h>
#include <gtkmm/image.h>

#include "bst.hpp"
#include "checkers.hpp"
#include "game.hpp"
#include "DrawGame.hpp"
#include "GameWindow.hpp"

Gtk::Button* set_but(const char *label, const Gtk::StockID& stock)
{
	using namespace Gtk;
	Button *button = manage(new Button(label));
	Image *img = manage(new Gtk::Image(stock,Gtk::ICON_SIZE_BUTTON));
	button->set_image(*img);
	button->set_label(label);

	return button;
}

LoadGameDialog::LoadGameDialog()
{
	using namespace Gtk;

	ButtonBox* actionArea = get_action_area();
	yesButton = set_but("Yes",Gtk::Stock::DIRECTORY);
	yesButton->signal_clicked().connect(sigc::mem_fun(*this,
			&LoadGameDialog::onYesButtonClicked) );
	noButton = set_but("No",Stock::NO);
	noButton->signal_clicked().connect(sigc::mem_fun(*this,
			&LoadGameDialog::onNoButtonClicked) );

	actionArea->pack_start(*noButton,false, false);
	actionArea->pack_start(*yesButton,false,false);

	show_all_children();
}

LoadGameDialog::~LoadGameDialog() {;}

void LoadGameDialog::onYesButtonClicked()
{
	DrawGame *theGame = new DrawGame();
	playAIvsAI(theGame,true);
	delete 	theGame;
}

void LoadGameDialog::onNoButtonClicked()
{
	hide();
}

GameWindow::GameWindow() {
	// TODO Auto-generated constructor stub
	goButton = set_but("GO!!",Gtk::Stock::APPLY);
	goButton->signal_clicked().connect(sigc::mem_fun(*this,
			&GameWindow::onGoButtonClicked));

	add(*goButton);
	show_all_children();
}

GameWindow::~GameWindow() {
	// TODO Auto-generated destructor stub
}

void GameWindow::onGoButtonClicked()
{
	LoadGameDialog lg;
	lg.run();
}

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app =
			Gtk::Application::create(argc, argv,
					"org.gtkmm.examples.base");
	Gtk::Settings::get_default()->property_gtk_button_images() = true;

	GameWindow window;
	return app->run(window);
}
