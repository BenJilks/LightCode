#pragma once
#include "content/settingspage.hpp"

namespace lc
{

    class SettingsManager
    {
    public:
        SettingsManager();

        const map<string, Setting> &operator[](string name) { return settings[name]; }
        SettingsPage *make_page(string name);
        void load();
        void save();

    private:
        void find_config_dir();
        void load_font(rapidjson::Value &font, Setting &setting);
        
        void load_setting(map<string, Setting> &setting, 
            rapidjson::Value &json);
        
        rapidjson::Value save_setting(map<string, Setting> &setting, 
            rapidjson::MemoryPoolAllocator<> &allocator);

        map<string, map<string, Setting>> settings;
        string config_dir = "";

    };

}
