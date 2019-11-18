#pragma once
#include <pangomm/attrlist.h>
#include <string>
#include <vector>
#include <regex>
using std::string;
using std::vector;

namespace lc
{

	class Syntax
	{
	public:
		Syntax();

		enum Type
		{
			Keyword,
			DataType,
			Number,
			String
		};

		struct Token
		{
			string pattern;
			Type type;
			std::regex reg;
		};
		
		void parse(string buffer);
		void add_token(string pattern, Type type);
		void apply_styles(Pango::AttrList &attrs);
	
	private:
		vector<Token> tokens;
		Pango::AttrList highlighting;
		int buffer_len;
		bool has_parsed;
	
	};

}

