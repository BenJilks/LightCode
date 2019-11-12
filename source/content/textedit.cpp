#include "content/textedit.hpp"
#include "options/optionseditor.hpp"
#include <fstream>
#include <vector>
using std::vector;
using namespace lc;

TextEdit::TextEdit() :
	file_path("")
{
	buffer = text.get_buffer();
	apply_settings();
	add(text);
}

TextEdit::TextEdit(string file_path) :
	file_path(file_path)
{
	buffer = text.get_buffer();
	open(file_path);
	apply_settings();
	add(text);
}

void TextEdit::apply_settings()
{
	auto font_setting = OptionsEditor::get_font();

	Pango::FontDescription desc;
	desc.set_family("Standard Symbols PS Bold");
	//desc.set_size(font_setting.second);
	printf("Setting font to: %s %i\n", font_setting.first.c_str(), font_setting.second);

	auto font = get_pango_context()->load_font(desc);
	text.set_font_map(font->get_font_map());
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
