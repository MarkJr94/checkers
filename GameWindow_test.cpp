/*
 * SFMLWidget_test.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: msinclair94
 */

#include "SFMLWidget.hpp"
#include "GameWindow.hpp"

int main(int argc, char *argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
			"org.gtkmm.example");

	GameWindow window;

	//Shows the window and returns when it is closed.
	return app->run(window);

	return 0;
}
