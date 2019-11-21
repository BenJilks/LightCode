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
		char c = source[pp];
		buffer += c;

		string symbol = string(&c, 1);
		if (isspace(c) ||
			is_line(pp, source) ||
			is_string(pp, source) || 
			is_keyword(symbols, pp, symbol))
		{
			string keyword = buffer.substr(0, buffer.length() - 1);
			is_keyword(keywords, pos, keyword);

			pos = pp + 1;
			buffer = "";
		}
		
		pp += 1;
	}

	has_parsed = true;
}

bool Syntax::is_keyword(vector<Token> &keywords, int pos, string &buffer)
{
	for (auto &keyword : keywords)
	{
		if (keyword.pattern == buffer)
		{
			highlight(pos, buffer.length(), keyword.type);
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
			highlight(pos, buffer.length(), pattern.type);
			return true;
		}
	}
	
	return false;
}

bool Syntax::is_line(int &pp, string &source)
{
	int pos = pp;
	for (auto &line : lines)
	{
		int length = line.pattern.length();
		std::string_view buffer(&source[pp], length);
		if (line.pattern == buffer)
		{
			for (;;)
			{
				char c = source[pp++];
				if (c == '\n')
					break;
			}

			highlight(pos, (pp - 1) - pos, line.type);
			return true;
		}
	}

	return false;
}

bool Syntax::is_string(int &pp, string &source)
{
	int pos = pp;
	for (auto &string : strings)
	{
		int start_len = string.first.length();
		int end_len = string.second.length();
		std::string_view start(&source[pp], start_len);
		if (string.first == start)
		{
			pp += start_len;
			for (;;)
			{
				std::string_view str(&source[pp], end_len);
				if (str == string.second)
				{
					pp += end_len;
					break;
				}
				pp += 1;
			}

			highlight(pos, pp - pos, string.type);
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

void Syntax::highlight(int pos, int len, Type type)
{
	auto attr = Pango::Attribute::
		create_attr_foreground(COLOUR(colours[type]));

	attr.set_start_index(pos);
	attr.set_end_index(pos + len);
	highlighting.insert(attr);
}

void Syntax::apply_styles(Pango::AttrList &attrs)
{
	if (has_parsed)
		attrs.splice(highlighting, 0, buffer_len);
}
