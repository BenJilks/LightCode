#pragma once
#include "options.hpp"
#include <gtkmm/dialog.h>
#include <gtkmm/fontbutton.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/stylecontext.h>
#include <string>
#include <tuple>
using std::string;
using std::pair;

namespace lc
{

    class OptionsEditor : public Gtk::Dialog, public Options
    {
    public:
        OptionsEditor();
        static inline pair<string, int> get_font() { return font; }
        static inline int get_tab_size() { return tab_size; }
    
        static void load_settings(rapidjson::Value &settings);
        static rapidjson::Value save_settings(
            rapidjson::MemoryPoolAllocator<> &allocator);
    
    private:

        void font_change();
        void tab_size_change();
        Gtk::FontButton *font_button;
        Gtk::SpinButton *tab_size_button;

        // Settings
        static pair<string, int>    font;
        static int                  tab_size;

    };

}
