#include "textedit.hpp"
#include <fstream>
#include <vector>
using std::vector;
using namespace lc;

TextEdit::TextEdit() :
	file_path("")
{
	buffer = text.get_buffer();
	add(text);
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
}

void TextEdit::clear()
{
	file_path = "";
	buffer->set_text("");
}

