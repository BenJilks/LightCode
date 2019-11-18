#pragma once
#include <pangomm/layout.h>
#include <gtkmm/stylecontext.h>
#include <functional>

namespace lc
{

    class CodeViewCursor
    {
    public:
        CodeViewCursor() {}
        CodeViewCursor(Glib::RefPtr<Pango::Layout> layout, 
            Gtk::Widget *parent);

        inline int get_index() const { return index; }
        inline bool is_in_blink() const { return in_blink; }
        inline void signal_updated(std::function<void()> callback) 
            { updated_callback = callback; }

        void update_blink();
        void apply_styles(Glib::RefPtr<Gtk::StyleContext> style, 
            Pango::AttrList &attrs);

    private:
        bool update_mouse_down(GdkEventButton* button_event);
        bool update_mouse_move(GdkEventMotion* motion_event);
        bool update_input(GdkEventKey* key_event);

        void input_mouse(int x, int y);
        void input_move_left_right(int key_code, int state);
        void input_move_up_down(int key_code, int state);
        int get_x();
        void moved(int from, int state);

        // Context info
        Glib::RefPtr<Pango::Layout> layout;
        std::function<void()> updated_callback;

        // Cursor info
        int index, last_x;
        int select_start;
        bool has_selection;
        bool in_blink;

    };

}
