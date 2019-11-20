#include "textedit/language/syntax.hpp"
#include <iostream>
#include <map>
#include <gdkmm/rgba.h>
using namespace lc;

Syntax::Syntax(string name)
{
	has_parsed = false;
}

static inline bool ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size()) 
		return false;

    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool Syntax::is_this_lang(string path) const
{
	for (const string &ext : extentions)
		if (ends_with(path, ext))
			return true;
	
	return false;
}

void Syntax::parse(string source)
{
	// Clear existing attributes
	highlighting = Pango::AttrList();
	buffer_len = source.length();

	int pp = 0, pos = 0;
	string buffer = "";
	while (pp < buffer_len)
	{
		char c = source[pp++];
		buffer += c;
		if (isspace(c))
		{
			pos = pp;
			buffer = "";
			continue;
		}

		string symbol = string(&c, 1);
		if (is_keyword(symbols, pp - 1, symbol) ||
			is_keyword(keywords, pos, buffer) ||
			/*is_pattern(pos, buffer) || */
			is_line(pos, pp, source, buffer) ||
			is_string(pos, pp, source, buffer))
		{
			pos = pp;
			buffer = "";
		}
	}

	has_parsed = true;
}

bool Syntax::is_keyword(vector<Token> &keywords, int pos, string &buffer)
{
	for (auto &keyword : keywords)
	{
		if (keyword.pattern == buffer)
		{
			highlight(pos, buffer, keyword.type);
			return true;
		}
	}
	
	return false;
}

bool Syntax::is_pattern(int pos, string &buffer)
{
	for (auto &pattern : patterns)
	{
		std::regex reg(pattern.pattern);
		if (std::regex_match(buffer, reg))
		{
			highlight(pos, buffer, pattern.type);
			return true;
		}
	}
	
	return false;
}

bool Syntax::is_line(int pos, int &pp, string &source, string &buffer)
{
	for (auto &line : lines)
	{
		if (line.pattern == buffer)
		{
			for (;;)
			{
				char c = source[pp++];
				if (c == '\n')
					break;
				buffer += c;
			}

			highlight(pos, buffer, line.type);
			return true;
		}
	}

	return false;
}

bool Syntax::is_string(int pos, int &pp, string &source, string &buffer)
{
	for (auto &string : strings)
	{
		if (string.first == buffer)
		{
			int len = string.second.length();
			for (;;)
			{
				std::string_view str(source.c_str() + pp, len);
				if (str == string.second)
				{
					buffer += str;
					pp += len;
					break;
				}
				buffer += source[pp++];
			}

			highlight(pos, buffer, string.type);
			return true;
		}
	}

	return false;
}

#define COLOUR(colour) \
    colour.get_red() * 65535, \
    colour.get_green() * 65535, \
    colour.get_blue() * 65535

#define lolz
// Test
static std::map<Syntax::Type, Gdk::RGBA> colours =
{
	std::make_pair(Syntax::Type::Comment, Gdk::RGBA("#b2e381")),
	std::make_pair(Syntax::Type::DataType, Gdk::RGBA("#81b7e3")),
	std::make_pair(Syntax::Type::Keyword, Gdk::RGBA("#ab1da1")),
	std::make_pair(Syntax::Type::Macro, Gdk::RGBA("#e643da")),
	std::make_pair(Syntax::Type::Number, Gdk::RGBA("#e64343")),
	std::make_pair(Syntax::Type::String, Gdk::RGBA("#e64343")),
	std::make_pair(Syntax::Type::Symbol, Gdk::RGBA("#FFFFFF")),
};

void Syntax::highlight(int pos, string &buffer, Type type)
{
	auto attr = Pango::Attribute::
		create_attr_foreground(COLOUR(colours[type]));

	attr.set_start_index(pos);
	attr.set_end_index(pos + buffer.length());
	highlighting.insert(attr);
}

void Syntax::apply_styles(Pango::AttrList &attrs)
{
	if (has_parsed)
		attrs.splice(highlighting, 0, buffer_len);
}
