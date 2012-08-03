/*
 * GameWindow.hpp
 *
 *  Created on: Jul 23, 2012
 *      Author: markjr
 */

#ifndef GAMEWINDOW_HPP_
#define GAMEWINDOW_HPP_

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/settings.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/stockid.h>
#include <gtkmm/image.h>


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

class GameWindow : public Gtk::Window {
public:
	GameWindow();
	virtual ~GameWindow();

protected:
	Gtk::Button* goButton;

	void onGoButtonClicked();
};

#endif /* GAMEWINDOW_HPP_ */
