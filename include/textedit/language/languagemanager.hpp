#pragma once
#include "syntax.hpp"
#include <rapidjson/document.h>
namespace json = rapidjson;

namespace lc
{

    class LanguageManager
    {
    public:
        LanguageManager() {}
        LanguageManager(string lang_folder);

        Syntax find_lang(string path);

    private:
        void parse_lang(json::Document &doc);

        void parse_token(Syntax &lang, json::Value &tokens, 
            const char *name, Syntax::Type type);

        void for_pattern(json::Value &patterns, const char *name,
            std::function<void(json::Value &)> on_pattern);

        void parse_pattern(vector<Syntax::Token> &tokens, 
            json::Value &patterns, const char *name, Syntax::Type type);
        
        void parse_double(vector<Syntax::Token> &tokens, 
            json::Value &patterns, const char *name, Syntax::Type type);

        vector<Syntax> langs;

    };

}
