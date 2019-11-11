#pragma once
#include <gtkmm/window.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>
#include "textedit.hpp"

namespace lc
{

	class Window : public Gtk::Window
	{
	public:
		Window();
	
	private:
		// File actions
		void on_file_new();
		void on_file_open();
		void on_file_save();
		void on_file_save_as();

		// Menubar
		Gtk::MenuBar menu_bar;
		Gtk::Box layout;
	
		// Content
		TextEdit editor;
	
	};

}
