#pragma once
#include "contentpage.hpp"
#include "settings/settings.hpp"
#include <gtkmm/label.h>
#include <gtkmm/grid.h>
#include <rapidjson/document.h>
#include <tuple>
#include <map>
#include <vector>
using std::pair;
using std::vector;
using std::map;

namespace lc
{

    class SettingBox
    {
    public:
        SettingBox(string name, Setting *setting);
        ~SettingBox();

        inline Gtk::Widget &get_first() { return label; }
        inline Gtk::Widget &get_second() { return *second; }

    private:
        void updated_int();
        void updated_string();
        void updated_font();

        Gtk::Label label;
        Gtk::Widget *second;
        Setting *setting;
    
    };

    class SettingsPage : public ContentPage
    {
    public:
        SettingsPage(string setting_name, std::map<string, Setting> *settings);
        ~SettingsPage();

        virtual string get_title() const { return setting_name + " Settings"; }
        virtual bool has_content() const { return true; }
        virtual void apply_settings(SettingsManager *settings) {}

    private:
        void build_page();
        void on_setting_changed(const Gtk::StateType& state_type);

        Gtk::Box settings_box;
        Gtk::Grid settings_grid;
        Gtk::Label settings_title;
        vector<SettingBox*> setting_boxes;

        std::map<string, Setting> *settings;
        string setting_name;

    };

}
