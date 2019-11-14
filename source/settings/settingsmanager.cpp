#include "settings/settingsmanager.hpp"
#include "content/settingspage.hpp"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include <pwd.h>
#include <sys/stat.h>
using namespace lc;

SettingsManager::SettingsManager()
{
    map<string, Setting> editor;
    editor["Font"] = Setting(SettingType::Font, "Monospace 12");
    editor["Tab Size"] = Setting(4, 0, 100);
    settings["Editor"] = editor;
}

SettingsPage *SettingsManager::make_page(string name)
{
    auto *setting = &settings[name];
    return new SettingsPage(name, setting);
}

void SettingsManager::load()
{
    if (config_dir == "")
        find_config_dir();
    
    // Open the file and find its length
    std::ifstream ifstream(config_dir + "config.json");
    if (ifstream.good())
    {
        rapidjson::IStreamWrapper stream(ifstream);

        // Parse the json stream
        rapidjson::Document settings_doc;
        settings_doc.ParseStream(stream);
        
        if (settings_doc.IsObject())
        {
            for (auto &setting : settings)
            {
                if (settings_doc.HasMember(setting.first.c_str()))
                {
                    auto &data = settings_doc[setting.first.c_str()];
                    if (data.IsObject())
                        load_setting(setting.second, data);
                }
            }
        }

        // Clean up
        ifstream.close();
    }
}

void SettingsManager::save()
{
    // Open the output stream
    std::ofstream ofstream(config_dir + "/config.json");
    rapidjson::OStreamWrapper stream(ofstream);
    rapidjson::Document settings_doc;

    // Write new json data
    auto &allocator = settings_doc.GetAllocator();
    settings_doc.SetObject();
    for (auto &setting : settings)
    {
        auto data = save_setting(setting.second, allocator);
        rapidjson::Value name(setting.first.c_str(), 
            setting.first.length(), allocator);
        
        settings_doc.AddMember(name, data, allocator);
    }

    // Write the file to stream
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(stream);
    settings_doc.Accept(writer);
    ofstream.close();
}

void SettingsManager::load_setting(map<string, Setting> &setting, 
    rapidjson::Value &json)
{
    for (auto &set : setting)
    {
        if (json.HasMember(set.first.c_str()))
        {
            auto &data = json[set.first.c_str()];
            switch(set.second.get_type())
            {
                case SettingType::Int: 
                    if (data.IsInt())
                        set.second.set_int(data.GetInt());
                    break;
                
                case SettingType::String: 
                    if (data.IsString())
                        set.second.set_string(data.GetString());
                    break;
                
                case SettingType::Font:
                    if (data.IsString())
                        set.second.set_font(data.GetString());
                    break;
            }
        }
    }
}

rapidjson::Value SettingsManager::save_setting(map<string, Setting> &setting, 
    rapidjson::MemoryPoolAllocator<> &alloc)
{
    rapidjson::Value json(rapidjson::kObjectType);
    for (auto &set : setting)
    {
        rapidjson::Value item;
        Setting &value = set.second;
        switch(value.get_type())
        {
            case SettingType::Int: 
                item.SetInt(value.get_int()); 
                break;

            case SettingType::String: 
                item.SetString(value.get_string().c_str(), 
                    value.get_string().length(), alloc);
                break;

            case SettingType::Font: 
            {
                item.SetString(value.get_font().c_str(), 
                    value.get_font().length(), alloc);
                break;
            }
        }

        rapidjson::Value name(set.first.c_str(), set.first.length(), alloc);
        json.AddMember(name, item, alloc);
    }

    return json;
}

void SettingsManager::find_config_dir()
{    
    // Find users config folder
    struct passwd *pw = getpwuid(getuid());
    config_dir = string(pw->pw_dir) + "/.config/lightcode/";

    // Create folders if they do not exist already
    mkdir(config_dir.c_str(), 0777);
}
