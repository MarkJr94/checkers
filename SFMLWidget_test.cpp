/*
 * SFMLWidget_test.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: msinclair94
 */

#include "SFMLWidget.hpp"

class ExampleWindow: public Gtk::Window {
public:
	ExampleWindow() :
			m_VBox(Gtk::ORIENTATION_VERTICAL), m_MyWidget(sf::VideoMode(800,800,32)), m_Button_Quit("Quit") {
		set_title("Custom Widget example");
		set_border_width(6);
		set_default_size(800, 800);

		add(m_VBox);
		auto msd = new GameWin(800,800);
		msd->bindGame(new Game(false,false));
		m_MyWidget.bindWin(msd);
		m_VBox.pack_start(m_MyWidget, Gtk::PACK_EXPAND_WIDGET);
		m_MyWidget.show();

		m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

		m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
		m_ButtonBox.set_border_width(6);
		m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
		m_Button_Quit.signal_clicked().connect(
				sigc::mem_fun(*this, &ExampleWindow::on_button_quit));

		show_all_children();
	}
	virtual ~ExampleWindow() {
	}

protected:
	//Signal handlers:
	void on_button_quit() {
		hide();
	}

	//Child widgets:
	Gtk::Box m_VBox;
	SFMLWidget m_MyWidget;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button m_Button_Quit;
};

int sfmlwTest(int argc, char * argv[]) {
    Gtk::Main kit(argc, argv); //Initialize Gtk

    Gtk::Window window; //The GTK window will be our top level Window

    //Our RenderWindow will never be below  640x480 (unless we explicitly change it)
    //but it may be more then that
    SFMLWidget ourRenderWindow(sf::VideoMode(640, 480));

    // Doesn't draw the renderWindow but makes it so it will draw when we add it to the window
    ourRenderWindow.show();

    //VBox is a vertical box, we're going to pack our render window and a button in here
    Gtk::VBox ourVBox;

    Gtk::Button ourButton("Hello I do nothing"); //Just a clickable button, it won't be doing anything
    ourButton.show();

    ourVBox.pack_start(ourRenderWindow); //Add ourRenderWindow to the top of the VBox

    //PACK_SHRINK makes the VBox only allocate enough space to show the button and nothing more
    ourVBox.pack_start(ourButton, Gtk::PACK_SHRINK);
    ourVBox.show();

    window.add(ourVBox); //Adds ourVBox to the window so it (and it's children) can be drawn

    Gtk::Main::run(window); //Draw the window
    return 0;
}

int main(int argc, char *argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
			"org.gtkmm.example");

	ExampleWindow window;

	//Shows the window and returns when it is closed.
	return app->run(window);

	return 0;
}
