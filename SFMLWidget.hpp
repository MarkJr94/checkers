/*
 * SFMLWidget.hpp
 *
 *  Created on: Sep 11, 2012
 *      Author: msinclair94
 */

#ifndef SFMLWIDGET_HPP_
#define SFMLWIDGET_HPP_

// SFML Includes
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Gtk Includes
#include <gtkmm.h>

// GLib Includes
#include <glibmm.h>

// GDK Includes
#include <gdkmm.h>
#include <gdk/gdkx.h>

// SFML Includes

#include <gdk/gdk.h>
#include <gdkmm/general.h>
#include <gtkmm.h>

#include "SFMLGame.hpp"

class SFMLWidget: public Gtk::Widget //, public sf::RenderWindow
{
public:
	friend class GameWindow;

	SFMLWidget(sf::VideoMode Mode);
	virtual ~SFMLWidget();

	void bindWin(SFMLGame* win) {
		delete _sfRenWin;
		_sfRenWin = win;
	}

private:
	sf::VideoMode m_vMode;

	virtual void on_size_request(Gtk::Requisition* requisition);
	virtual void on_size_allocate(Gtk::Allocation& allocation);
	virtual void on_map();
	virtual void on_unmap();
	virtual void on_realize();
	virtual void on_unrealize();
	virtual bool on_idle();
	virtual bool on_expose_event(GdkEventExpose* event);
	void DrawObjects();

	Glib::RefPtr<Gdk::Window> _gdkWindow;

	SFMLGame* _sfRenWin;

};

#endif /* SFMLWIDGET_HPP_ */
