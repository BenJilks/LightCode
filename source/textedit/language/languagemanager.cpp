#include "textedit/language/languagemanager.hpp"
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <filesystem>
using namespace lc;
namespace fs = std::filesystem;

LanguageManager::LanguageManager(string lang_folder)
{
    for (auto lang_file : fs::directory_iterator(lang_folder))
    {
        std::ifstream i_stream(lang_file.path());
        json::IStreamWrapper stream(i_stream);
        
        json::Document doc;
        doc.ParseStream(stream);
        parse_lang(doc);

        i_stream.close();
    }
}

Syntax LanguageManager::find_lang(string path)
{
    for (const Syntax &lang : langs)
        if (lang.is_this_lang(path))
            return lang;
    
    Syntax blank("Plain Text");
    return blank;
}

void LanguageManager::parse_lang(json::Document &doc)
{
    auto name = doc["name"].GetString();
    auto extensions = doc["extensions"].GetArray();

    // Read file extentions
    Syntax lang(name);
    for (auto &ext : extensions)
        lang.add_extention(ext.GetString());
    
    // Read token patterns
    auto &tokens = doc["tokens"];
    parse_token(lang, tokens, "keyword", Syntax::Keyword);
    parse_token(lang, tokens, "datatype", Syntax::DataType);
    parse_token(lang, tokens, "comment", Syntax::Comment);
    parse_token(lang, tokens, "macro", Syntax::Macro);
    parse_token(lang, tokens, "number", Syntax::Number);
    parse_token(lang, tokens, "string", Syntax::String);
    parse_token(lang, tokens, "symbol", Syntax::Symbol);
    langs.push_back(lang);
}

void LanguageManager::parse_token(Syntax &lang, json::Value &tokens,
    const char *name, Syntax::Type type)
{
    if (tokens.HasMember(name))
    {
        auto &patterns = tokens[name];
        if (patterns.IsObject())
        {
            parse_pattern(lang.get_keywords(), patterns, "keyword", type);
            parse_pattern(lang.get_patterns(), patterns, "pattern", type);
            parse_pattern(lang.get_lines(), patterns, "line", type);
            parse_double(lang.get_strings(), patterns, "string", type);
            parse_pattern(lang.get_symbols(), patterns, "symbol", type);
        }
    }
}

void LanguageManager::for_pattern(json::Value &patterns, const char *name,
    std::function<void(json::Value &)> on_pattern)
{
    if (patterns.HasMember(name))
    {
        auto &pattern = patterns[name];
        if (pattern.IsArray())
        {
            for (auto &value : pattern.GetArray())
                on_pattern(value);
        }
    }    
}

void LanguageManager::parse_pattern(vector<Syntax::Token> &tokens, 
    json::Value &patterns, const char *name, Syntax::Type type)
{
    Syntax::Token token;
    token.type = type;

    for_pattern(patterns, name, [&token, &tokens](json::Value &pattern)
    {
        if (pattern.IsString())
        {
            token.pattern = pattern.GetString();
            tokens.push_back(token);
        }
    });
}

void LanguageManager::parse_double(vector<Syntax::Token> &tokens, 
    json::Value &patterns, const char *name, Syntax::Type type)
{
    Syntax::Token token;
    token.type = type;

    for_pattern(patterns, name, [&token, &tokens](json::Value &pattern)
    {
        if (pattern.IsArray())
        {
            token.first = pattern[0].GetString();
            token.second = pattern[1].GetString();
            tokens.push_back(token);
        }
    });
}
