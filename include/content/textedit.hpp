#pragma once
#include "contentpage.hpp"
#include "textedit/codeview.hpp"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/cssprovider.h>
#include <string>
using std::string;

namespace lc
{

	class TextEdit : public ContentPage
	{
	public:
		TextEdit(string file_path, Syntax syntax);
		TextEdit();
		
		inline const string &get_file_path() const { return file_path; };
		inline bool has_file_path() const { return file_path == ""; }

		virtual void open(string file);
		void clear();
		virtual string get_title() const;
		virtual bool has_content() const { return content_flag; }
		virtual void apply_settings(SettingsManager *settings);
	
	private:
		string file_path;
		CodeView code;
		bool content_flag = false;
	
	};

}
