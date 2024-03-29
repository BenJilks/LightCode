#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/cssprovider.h>
#include "window.hpp"
using namespace lc;

int main(int argc, char *argv[])
{
	// Create new gtk app
	auto app = Gtk::Application::create(
		argc, argv, "org.benjilks.lc");

	// Create keyboard shortcuts
	app->set_accel_for_action("New", "<Primary>n");
	app->set_accel_for_action("Open", "<Primary>o");
	app->set_accel_for_action("Save", "<Primary>s");
	app->set_accel_for_action("SaveAs", "<Primary>v");

	// Run the app
	Window window;
	return app->run(window);
}
