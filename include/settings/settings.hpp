#pragma once
#include <string>
#include <tuple>
using std::string;
using std::pair;

namespace lc
{

    enum class SettingType
    {
        None,
        Int,
        String,
        Font
    };

    class Setting
    {
    public:
        Setting() : 
            type(SettingType::None) {}
        
        Setting(SettingType type) : 
            type(type) {}
        
        Setting(int i, int min, int max) : 
            type(SettingType::Int), i(i), 
            i_min(min), i_max(max) {}
        
        Setting(string str) : 
            type(SettingType::String), str(str) {}
        
        Setting(pair<string, int> font) : 
            type(SettingType::Font), font(font) {}
        
        inline SettingType get_type() const { return type; }
        inline auto get_int() const { return i; }
        inline auto get_min() const { return i_min; }
        inline auto get_max() const { return i_max; }
        inline auto get_string() const { return str; }
        inline auto get_font() const { return font; }

        inline void set_int(int i) { this->i = i; }
        inline void set_string(string str) { this->str = str; }
        inline void set_font(pair<string, int> font) { this->font = font; }

    private:
        SettingType type;

        int i, i_min, i_max;
        string str;
        pair<string, int> font;
    };

}
