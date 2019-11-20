#include "content/textedit.hpp"
#include "content/contentmanager.hpp"
#include "settings/settingsmanager.hpp"
#include <gtkmm/cssprovider.h>
#include <fstream>
#include <vector>
using std::vector;
using namespace lc;

TextEdit::TextEdit(string file_path, Syntax syntax) :
	file_path(file_path), code(syntax)
{
	if (file_path != "")
		open(file_path);
	
	add(code);
	title.update_title(get_title());
}

TextEdit::TextEdit() :
	TextEdit("", Syntax("Plain Text")) {}

void TextEdit::apply_settings(SettingsManager *settings)
{
	auto editor = (*settings)["Editor"];
	auto font = editor["Font"].get_font();
	auto tab_size = editor["Tab Size"].get_int();

	code.set_font(font);
	code.set_tab_size(tab_size);
}

string TextEdit::get_title() const
{
	// If there's no file open
	if (file_path == "")
		return "Untitled Document";

	// Find the last '/'
	int i;
	for (i = file_path.length(); i >= 0; --i)
	{
		if (file_path[i] == '/')
		{
			i += 1;
			break;
		}
	}
	
	// Create a sub string from that point
	return file_path.substr(i, file_path.length() - i);
}

void TextEdit::open(string file)
{
	// Open the file stream and find its length
	std::ifstream stream(file);
	stream.seekg(0L, std::ifstream::end);
	int len = stream.tellg();
	stream.seekg(0L, std::ifstream::beg);
	
	// Read the file into a buffer, then close it
	vector<char> data(len);
	stream.read(&data[0], len);
	stream.close();
	
	// Load the data into the text buffer
	code.set_buffer(string(&data[0], len));
	file_path = file;
	content_flag = true;
	title.update_title(get_title());
}

void TextEdit::clear()
{
	file_path = "";
	content_flag = false;
	//buffer->set_text("");
}
