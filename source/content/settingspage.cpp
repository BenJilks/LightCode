#include "content/settingspage.hpp"
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/fontbutton.h>
using namespace lc;

SettingBox::SettingBox(string name, Setting *setting) :
    label(name), setting(setting)
{
    // Select the correct input for the type
    switch (setting->get_type())
    {
        case SettingType::Int: 
        {
            auto number = new Gtk::SpinButton;
            number->set_range(setting->get_min(), setting->get_max());
            number->set_increments(1, 4);
            number->set_value(setting->get_int());
            number->signal_changed().connect(
                sigc::mem_fun(this, &SettingBox::updated_int));
            second = number;
            break;
        }
        
        case SettingType::String: 
        {
            auto text = new Gtk::Entry; 
            text->set_text(setting->get_string());
            text->signal_changed().connect(
                sigc::mem_fun(this, &SettingBox::updated_string));
            second = text;
            break;
        }

        case SettingType::Font:
        {
            auto font_button = new Gtk::FontButton;
            font_button->set_use_font(true);
            font_button->set_font_name(setting->get_font());
            font_button->signal_font_set().connect(
                sigc::mem_fun(this, &SettingBox::updated_font));
            second = font_button;
            break;
        }
    }

    // Apply secondary settings
    label.set_alignment(Gtk::Align::ALIGN_START);
    second->set_hexpand(true);
}

SettingBox::~SettingBox()
{
    delete second;
}

void SettingBox::updated_int()
{
    auto number = (Gtk::SpinButton*)second;
    setting->set_int(number->get_value_as_int());
}

void SettingBox::updated_string()
{
    auto text = (Gtk::Entry*)second;
    setting->set_string(text->get_text());
}

void SettingBox::updated_font()
{
    // Get font information
    auto font_button = (Gtk::FontButton*)second;
    auto font_map = font_button->get_font_name();
    setting->set_font(font_map);
}

SettingsPage::SettingsPage(string setting_name, std::map<string, Setting> *settings) :
    setting_name(setting_name), settings(settings)
{
    title.update_title(get_title());
    build_page();
}

SettingsPage::~SettingsPage()
{
    for (auto setting_box : setting_boxes)
        delete setting_box;
}

void SettingsPage::build_page()
{
    // Configure title - settings box
    settings_title.set_text(setting_name + " Settings");
    settings_box.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
    settings_box.set_spacing(40);
    settings_box.set_margin_top(20);
    settings_box.pack_start(settings_title, Gtk::PackOptions::PACK_SHRINK);
 
    // Config settings grid where all the options will go
    settings_grid.set_row_spacing(10);
    settings_grid.set_column_spacing(40);
    settings_grid.set_margin_left(80);
    settings_grid.set_margin_right(80);

    // Add all the settings to the grid
    int y = 0;
    for (auto &setting : *settings)
    {
        auto box = new SettingBox(setting.first, &setting.second);
        settings_grid.attach(box->get_first(), 0, y, 1, 1);
        settings_grid.attach(box->get_second(), 1, y, 1, 1);
        setting_boxes.push_back(box);
        box->get_second().signal_state_changed().connect(
            sigc::mem_fun(this, &SettingsPage::on_setting_changed));
        y += 1;
    }

    // Add the grid to the box and the box to the page
    settings_box.pack_start(settings_grid);
    add(settings_box);
}

void SettingsPage::on_setting_changed(const Gtk::StateType& state_type)
{
    apply_callback();
}
