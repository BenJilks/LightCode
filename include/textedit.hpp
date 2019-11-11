#pragma once
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <string>
using std::string;

namespace lc
{

	class TextEdit : public Gtk::ScrolledWindow
	{
	public:
		TextEdit();
		
		inline const string &get_file_path() const { return file_path; };
		inline bool has_file_path() const { return file_path == ""; }

		void open(string file);
		void clear();
	
	private:
		string file_path;
		Gtk::TextView text;
		Glib::RefPtr<Gtk::TextBuffer> buffer;
	
	};

}
