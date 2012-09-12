/*
 * SFMLWidget.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: msinclair94
 */

#include "SFMLWidget.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <gtk-3.0/gdk/gdkx.h>


#include "SFMLWidget.hpp"

void SFMLWidget::DrawObjects()
{
    _sfRenWin->draw();
}

bool SFMLWidget::on_idle()
{
    if(_gdkWindow)
    {
        DrawObjects();
        _sfRenWin->Display();
    }

    return true;
}

SFMLWidget::SFMLWidget(sf::VideoMode Mode)
    : _sfRenWin(NULL)
{
	set_has_window(false) ;

    Glib::signal_idle().connect( sigc::mem_fun(*this, &SFMLWidget::on_idle) );
}

SFMLWidget::~SFMLWidget()
{
	delete _sfRenWin;
}

void SFMLWidget::on_size_request(Gtk::Requisition* requisition)
{
    *requisition = Gtk::Requisition();

    requisition->width = _sfRenWin->GetWidth();
    requisition->height = _sfRenWin->GetHeight();
}

void SFMLWidget::on_size_allocate(Gtk::Allocation& allocation)
{
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though
    //we might get more)

    this->set_allocation(allocation);

    if(_gdkWindow)
    {
//        _gdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height() );
    	_gdkWindow->move(allocation.get_x(), allocation.get_y());
//        _sfRenWin->SetSize(allocation.get_width(), allocation.get_height());
//        _sfRenWin->SetPosition(allocation.get_x(),allocation.gset_y());
    }
}

void SFMLWidget::on_map()
{
    Gtk::Widget::on_map();
}

void SFMLWidget::on_unmap()
{
    Gtk::Widget::on_unmap();
}

void SFMLWidget::on_realize()
{
    Gtk::Widget::on_realize();

    if(!_gdkWindow)
    {
        //Create the GdkWindow:
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));

        Gtk::Allocation allocation = get_allocation();

        //Set initial position and size of the Gdk::Window:
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();

        attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;


        _gdkWindow = Gdk::Window::create(get_window() /* parent */, &attributes,
                GDK_WA_X | GDK_WA_Y);
//        unset_flags(Gtk::NO_WINDOW);
        set_has_window(true);
        set_window(_gdkWindow);

        //set colors

        //make the widget receive expose events
        _gdkWindow->set_user_data(gobj());

        ///Reference: http://www.nabble.com/Win32-HWND-td20494257.html
        ///This is platform specific, compiling on Linux/MacOS will require a different Window Handle
        _sfRenWin->sf::RenderWindow::Create(GDK_WINDOW_XID(_gdkWindow->gobj()));
    }
}

void SFMLWidget::on_unrealize()
{
  _gdkWindow.clear();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool SFMLWidget::on_expose_event(GdkEventExpose* event)
{
    if(_gdkWindow)
    {
        DrawObjects();
        _sfRenWin->Display();
    }
    return true;
}
