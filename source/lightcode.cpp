#include <iostream>
#include <gtkmm/application.h>
#include "window.hpp"
using namespace lc;

int main(int argc, char *argv[])
{
	// Create new gtk app
	auto app = Gtk::Application::create(
		argc, argv, "org.benjilks.lc");
	
	// Run the app
	Window window;
	return app->run(window);
}

