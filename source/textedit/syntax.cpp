#include "textedit/syntax.hpp"
#include <iostream>
using namespace lc;

Syntax::Syntax()
{
	has_parsed = false;
}

void Syntax::add_token(string pattern, Type type)
{
	Token token;
	token.pattern = pattern;
	token.type = type;
	token.reg = std::regex(pattern);
	tokens.push_back(token);
}

void Syntax::parse(string buffer)
{
	// Clear existing attributes
	highlighting = Pango::AttrList();
	buffer_len = buffer.length();

	for (auto &token : tokens)
	{
		// Compile regex expression
		auto reg = token.reg;
		std::smatch match;

		// Find all instances of token
		string remaining = buffer;
		int curr_index = 0;
		while (std::regex_search(remaining, match, reg))
		{
			auto attr = Pango::Attribute::
				create_attr_foreground(0, 65535, 0);			

			int start = match.position();
			int end = start + match.length();
			attr.set_start_index(start + curr_index);
			attr.set_end_index(end + curr_index);
			highlighting.insert(attr);
			has_parsed = true;

			remaining = match.suffix().str();
			curr_index += end;
		}
	}
}

void Syntax::apply_styles(Pango::AttrList &attrs)
{
	if (has_parsed)
		attrs.splice(highlighting, 0, buffer_len);
}

