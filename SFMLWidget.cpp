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
//#include <gdk/gdkx.h>
#include <gtk-3.0/gdk/gdkx.h>


#include "SFMLWidget.hpp"

void SFMLWidget::DrawObjects()
{
//    m_renWin->Draw(m_tempSprite);
    m_renWin->draw();
}

bool SFMLWidget::on_idle()
{
    if(m_refGdkWindow)
    {
        DrawObjects();
        m_renWin->Display();
    }

    return true;
}

SFMLWidget::SFMLWidget(sf::VideoMode Mode)
    : m_renWin(NULL)
{
//    set_flags(Gtk::NO_WINDOW);
	set_has_window(false) ;
    m_tempImage.LoadFromFile("img.jpg");
    m_tempSprite.SetImage(m_tempImage);
    m_tempSprite.SetPosition(50, 50);

    Glib::signal_idle().connect( sigc::mem_fun(*this, &SFMLWidget::on_idle) );
}

SFMLWidget::~SFMLWidget()
{
	delete m_renWin;
}

void SFMLWidget::on_size_request(Gtk::Requisition* requisition)
{
    *requisition = Gtk::Requisition();

    requisition->width = m_renWin->GetWidth();
    requisition->height = m_renWin->GetHeight();
}

void SFMLWidget::on_size_allocate(Gtk::Allocation& allocation)
{
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though
    //we might get more)

    this->set_allocation(allocation);

    if(m_refGdkWindow)
    {
        m_refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height() );
        m_renWin->SetSize(allocation.get_width(), allocation.get_height());
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

    if(!m_refGdkWindow)
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


        m_refGdkWindow = Gdk::Window::create(get_window() /* parent */, &attributes,
                GDK_WA_X | GDK_WA_Y);
//        unset_flags(Gtk::NO_WINDOW);
        set_has_window(true);
        set_window(m_refGdkWindow);

        //set colors
//        modify_bg(Gtk::STATE_NORMAL , Gdk::Color("red"));
//        modify_fg(Gtk::STATE_NORMAL , Gdk::Color("blue"));

        //make the widget receive expose events
        m_refGdkWindow->set_user_data(gobj());

        ///Reference: http://www.nabble.com/Win32-HWND-td20494257.html
        ///This is platform specific, compiling on Linux/MacOS will require a different Window Handle
//        this->sf::RenderWindow::Create(reinterpret_cast<HWND>(GDK_WINDOW_HWND(m_refGdkWindow->gobj())));
        m_renWin->sf::RenderWindow::Create(GDK_WINDOW_XID(m_refGdkWindow->gobj()));
//        this->sf::RenderWindow::Create(GDK_DRAWABLE_ID(m_refGdkWindow->gobj()));
    }
}

void SFMLWidget::on_unrealize()
{
  m_refGdkWindow.clear();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool SFMLWidget::on_expose_event(GdkEventExpose* event)
{
    if(m_refGdkWindow)
    {
        DrawObjects();
        m_renWin->Display();
    }
    return true;
}
