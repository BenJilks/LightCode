#include "textedit/codeviewcursor.hpp"
#include "textedit/codeview.hpp"
#include <gdkmm/event.h>
#include <gtkmm/widget.h>
using namespace lc;

CodeViewCursor::CodeViewCursor(Glib::RefPtr<Pango::Layout> layout, 
    Gtk::Widget *parent) :
    layout(layout)
{
    // Set default values
    index = 0;
    last_x = 0;
    select_start = 0;
    has_selection = false;
    in_blink = false;

    // Connect signals
    parent->signal_key_press_event().connect(
        sigc::mem_fun(this, &CodeViewCursor::update_input));

    parent->signal_button_press_event().connect(
        sigc::mem_fun(this, &CodeViewCursor::update_mouse_down));

    parent->signal_motion_notify_event().connect(
        sigc::mem_fun(this, &CodeViewCursor::update_mouse_move));
}

#define COLOUR(colour) \
    colour.get_red() * 65535, \
    colour.get_green() * 65535, \
    colour.get_blue() * 65535

void CodeViewCursor::apply_styles(Glib::RefPtr<Gtk::StyleContext> style, 
    Pango::AttrList &attrs)
{
    if (has_selection)
    {
        // Fetch GTK theme colours
        auto background_colour = style->get_background_color(Gtk::STATE_FLAG_SELECTED);
        auto text_colour = style->get_color(Gtk::STATE_FLAG_SELECTED);
        int start = MIN(select_start, index);
        int end = MAX(select_start, index);

        // Create attrs
        auto background = Pango::Attribute::
            create_attr_background(COLOUR(background_colour));

        auto text = Pango::Attribute::
            create_attr_foreground(COLOUR(text_colour));

        // Set start and end locations
        background.set_start_index(start);
        background.set_end_index(end);
        attrs.insert(background);

        text.set_start_index(start);
        text.set_end_index(end);
        attrs.insert(text);
    }
}

void CodeViewCursor::update_blink()
{
    in_blink = !in_blink;
}

bool CodeViewCursor::update_input(GdkEventKey* key_event)
{
    input_move_left_right(key_event->keyval, key_event->state);
    input_move_up_down(key_event->keyval, key_event->state);
    index = CLAMP(index, 0, layout->get_text().length());

    return true;
}

void CodeViewCursor::input_move_left_right(int key_code, int state)
{
    int direction = 0;
    switch (key_code)
    {
        case GDK_KEY_Left: direction = -1; break;
        case GDK_KEY_Right: direction = 1; break;
        default: return;
    }

    // Move the postion left or right
    int from = index;
    int trailing;
    layout->move_cursor_visually(false, index, 0, direction, 
        index, trailing);
    index += trailing;
    last_x = get_x();

    moved(from, state);
}

void CodeViewCursor::input_move_up_down(int key_code, int state)
{
    int direction = 0;
    switch (key_code)
    {
        case GDK_KEY_Up: direction = -1; break;
        case GDK_KEY_Down: direction = 1; break;
        default: return;
    }

    // Find the current line
    int line_index, x_pos;
    int trailing;
    layout->index_to_line_x(index, 0, line_index, x_pos);

    // Get the index of the next line in the same position
    int from = index;
    auto line = layout->get_line(line_index + direction);
    line->x_to_index(last_x, index, trailing);
    index += trailing;

    moved(from, state);
}

void CodeViewCursor::input_mouse(int x, int y)
{
    int trailing;
    layout->xy_to_index(x * 1024, y * 1024, index, trailing);
    index += trailing;
    last_x = get_x();
}

bool CodeViewCursor::update_mouse_down(GdkEventButton* button_event)
{
    int from = index;
    input_mouse(button_event->x, button_event->y);
    moved(from, button_event->state);

    return true;
}

bool CodeViewCursor::update_mouse_move(GdkEventMotion* motion_event)
{
    if (motion_event->state & GDK_BUTTON1_MASK)
    {
        if (!has_selection)
        {
            select_start = index;
            has_selection = true;
        }

        input_mouse(motion_event->x, motion_event->y);
        if (updated_callback)
            updated_callback();
    }

    return true;
}

int CodeViewCursor::get_x()
{
    auto pos = layout->get_cursor_weak_pos(index);
    return pos.get_x();
}

void CodeViewCursor::moved(int from, int state)
{
    if (state & GDK_SHIFT_MASK && !has_selection)
    {
        select_start = from;
        has_selection = true;
    }
    else if (!(state & GDK_SHIFT_MASK) && has_selection)
    {
        has_selection = false;
    }

    if (updated_callback)
        updated_callback();
}
