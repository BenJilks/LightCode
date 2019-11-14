#include "content/textedit.hpp"
#include "content/contentmanager.hpp"
#include "settings/settingsmanager.hpp"
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
	
	add(text);
	title.update_title(get_title());
}

TextEdit::TextEdit() :
	TextEdit("") {}

void TextEdit::apply_settings(SettingsManager *settings)
{
	auto editor = (*settings)["Editor"];
	auto font = editor["Font"].get_font();
	auto tab_size = editor["Tab Size"].get_int();

	Pango::FontDescription font_desc(font);
	string css = 
		"* { "
		"	font-family: \"" + font_desc.get_family() + "\";"
		"	font-size: " + std::to_string(font_desc.get_size() / 1024) + "px;"
		"}";
	printf("%s\n", css.c_str());
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
	title.update_title(get_title());
}

void TextEdit::clear()
{
	file_path = "";
	content_flag = false;
	buffer->set_text("");
}
