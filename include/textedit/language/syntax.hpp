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
		Syntax(string name);

		enum Type
		{
			None,
			Keyword,
			DataType,
			Comment,
			Macro,
			Number,
			String,
			Symbol
		};

		struct Token
		{
			string pattern;
			string first;
			string second;
			Type type;
		};

		void parse(string buffer);
		void apply_styles(Pango::AttrList &attrs);

		bool is_this_lang(string path) const;
		void add_extention(string ext) { extentions.push_back(ext); }
		inline auto &get_keywords() { return keywords; }
		inline auto &get_patterns() { return patterns; }
		inline auto &get_lines() { return lines; }
		inline auto &get_strings() { return strings; }
		inline auto &get_symbols() { return symbols; }

	private:
		bool is_keyword(vector<Token> &keywords, int pos, string &buffer);
		bool is_pattern(int pos, string &buffer);
		bool is_line(int pos, int &pp, string &source, string &buffer);
		bool is_string(int pos, int &pp, string &source, string &buffer);
		void highlight(int pos, string &buffer, Type type);

		string name;
		vector<string> extentions;
		vector<Token> keywords;
		vector<Token> patterns;
		vector<Token> lines;
		vector<Token> strings;
		vector<Token> symbols;

		Pango::AttrList highlighting;
		int buffer_len;
		bool has_parsed;
	
	};

}

