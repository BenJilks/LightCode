#pragma once
#include <gtkmm/dialog.h>
#include <gtkmm/fontbutton.h>
#include <string>
#include <tuple>
using std::string;
using std::pair;

namespace lc
{

    class OptionsEditor : public Gtk::Dialog
    {
    public:
        OptionsEditor();

        static void load();
        static inline pair<string, int> get_font() { return font; }
    
    private:
        void font_change();
        Gtk::FontButton *font_button;

        // Settings
        static pair<string, int> font;

    };

}
