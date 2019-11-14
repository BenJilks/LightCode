#include "content/textedit.hpp"
#include "options/optionseditor.hpp"
#include <gtkmm/cssprovider.h>
#include <fstream>
#include <vector>
using std::vector;
using namespace lc;

TextEdit::TextEdit(string file_path) :
	file_path(file_path)
{
	buffer = text.get_buffer();
	if (file_path != "")
		open(file_path);

	// Create css provider
	auto context = text.get_style_context();
	css_provider = Gtk::CssProvider::create();
	context->add_provider(css_provider, 
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	
	apply_settings();
	add(text);
}

TextEdit::TextEdit() :
	TextEdit("") {}

void TextEdit::apply_settings()
{
	auto font = OptionsEditor::get_font();
	auto tab_size = OptionsEditor::get_tab_size();

	string css = 
		"* { "
		"	font-family: \"" + font.first + "\";"
		"	font-size: " + std::to_string(font.second) + "px;"
		"}";
	css_provider->load_from_data(css);
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
	buffer->set_text(string(&data[0], len));
	file_path = file;
	content_flag = true;
}

void TextEdit::clear()
{
	file_path = "";
	content_flag = false;
	buffer->set_text("");
}
