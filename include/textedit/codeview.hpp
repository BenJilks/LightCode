#pragma once
#include "codeviewcursor.hpp"
#include <gtkmm/widget.h>
#include <string>
using std::string;

namespace lc
{

    class CodeView : public Gtk::Widget
    {
    public:
        CodeView(string buffer);
        ~CodeView();

        void set_font(string font_name);
        void set_tab_size(int size);
        void set_buffer(string str);

    protected:
        void on_realize() override;
        void on_unrealize() override;
        bool on_button_press_event(GdkEventButton* button_event) override;
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    private:
        void draw_buffer(const Cairo::RefPtr<Cairo::Context>& cr);
        void draw_cursor(const Cairo::RefPtr<Cairo::Context>& cr, 
            CodeViewCursor *cursor);
        bool on_blink_update();

        void update_scroll();

        Glib::RefPtr<Gdk::Window> window;
        Glib::RefPtr<Pango::Layout> layout;
        CodeViewCursor *cursor;

        string buffer;
        int tab_size;

    };

}
