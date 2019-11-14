#include "options/optionseditor.hpp"
#include <gtkmm/builder.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/fontchooserdialog.h>
#include <gtkmm/cssprovider.h>
using namespace lc;

pair<string, int>   OptionsEditor::font = std::make_pair("Monospace", 12);
int                 OptionsEditor::tab_size = 4;

void OptionsEditor::load_settings(rapidjson::Value &settings)
{
    // Load settings from json
    rapidjson::Value &font_setting = settings["font"];
    rapidjson::Value &tab_size_setting = settings["tab_size"];

    if (font_setting.IsArray() && font_setting.Size() >= 2)
    {
        rapidjson::Value &font_name_setting = font_setting[0];
        rapidjson::Value &font_size_setting = font_setting[1];

        if (font_name_setting.IsString() && font_size_setting.IsInt())
        {
            font = std::make_pair(
                font_name_setting.GetString(), 
                font_size_setting.GetInt());
        }
    }

    if (tab_size_setting.IsInt()) tab_size = tab_size_setting.GetInt();
}

rapidjson::Value OptionsEditor::save_settings(
    rapidjson::MemoryPoolAllocator<> &allocator)
{
    rapidjson::Value editor(rapidjson::kObjectType);
    rapidjson::Value font_setting(rapidjson::kArrayType);
    rapidjson::Value font_name_setting(rapidjson::kStringType);
    rapidjson::Value font_size_setting(font.second);
    rapidjson::Value tab_size_setting(tab_size);

    font_name_setting.SetString(font.first.c_str(), font.first.length(), allocator);
    font_setting.PushBack(font_name_setting, allocator);
    font_setting.PushBack(font_size_setting, allocator);
    editor.AddMember("font", font_setting, allocator);
    editor.AddMember("tab_size", tab_size_setting, allocator);
    return editor;
}

void OptionsEditor::font_change()
{
    auto font_map = font_button->get_font_name();

    int i;
    for (i = font_map.length(); i >= 0; --i)
        if (isspace(font_map[i]))
            break;
    
    string size_str = font_map.substr(i, font_map.length() - i);
    font.first = font_map.substr(0, i);
    font.second = std::atoi(size_str.c_str());
}

void OptionsEditor::tab_size_change()
{
    tab_size = tab_size_button->get_value_as_int();
}

OptionsEditor::OptionsEditor()
{
    set_default_size(200, 200);
    set_title("Options - Editor");

    auto ui = Gtk::Builder::create();
    ui->add_from_file("../test.glade");

    Gtk::Box *cont;
    ui->get_widget("cont", cont);

    // Set font settings
    ui->get_widget("font", font_button);
    font_button->set_use_font(true);
    font_button->set_font_name(font.first + " " + std::to_string(font.second));
    font_button->signal_font_set().connect(
        sigc::mem_fun(*this, &OptionsEditor::font_change));

    // Set tab size settings
    ui->get_widget("tab_size", tab_size_button);
    tab_size_button->set_range(0, 100);
    tab_size_button->set_increments(1, 4);
    tab_size_button->set_value(tab_size);
    tab_size_button->signal_changed().connect(
        sigc::mem_fun(*this, &OptionsEditor::tab_size_change));

    get_content_area()->add(*cont);
    show_all_children();
}
