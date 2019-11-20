#pragma once
#include <gtkmm/window.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>
#include <gtkmm/paned.h>
#include "content/contentmanager.hpp"
#include "settings/settingsmanager.hpp"
#include "filemanager/filemanager.hpp"
#include "textedit/language/languagemanager.hpp"

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

		// Options actions
		void options_editor();

		// Menubar
		Gtk::MenuBar menu_bar;
		Gtk::Box layout;
		Gtk::Paned paned;
		ContentManager content;
		FileManager file_manager;
		LanguageManager lang_manager;
		SettingsManager settings;
	
	};

}
