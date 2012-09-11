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
//#include <gtkmm/widget.h>
#include <gtkmm.h>

// GLib Includes
//#include <glibmm/objectbase.h>
#include <glibmm.h>

// GDK Includes
#include <gdkmm.h>
//#include <gdkmm/window.h>
//#include <gdkmm/event.h>
#include <gdk/gdkx.h>



#include <SFML/Graphics.hpp>
//#include <gdk/gdkwin32.h>
#include <gdk/gdk.h>
#include <gdkmm/general.h>
#include <gtkmm.h>

#include "GameWin.hpp"

class SFMLWidget : public Gtk::Widget//, public sf::RenderWindow
{
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

        Glib::RefPtr<Gdk::Window> m_refGdkWindow;

        sf::Image m_tempImage;
        sf::Sprite m_tempSprite;

        BaseWin* m_renWin;
    public:
        SFMLWidget(sf::VideoMode Mode);
        virtual ~SFMLWidget();

        void bindWin(BaseWin* win) {
        	delete m_renWin;

        	m_renWin = win;
        }
};

#endif /* SFMLWIDGET_HPP_ */
