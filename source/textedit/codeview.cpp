#include "textedit/codeview.hpp"
#include <gtkmm/adjustment.h>
#include <gtkmm.h>
using namespace lc;

CodeView::CodeView(string buffer) :
    Glib::ObjectBase("CodeView"),
    Gtk::Widget(),
    buffer(buffer), tab_size(4)
{
    set_has_window(true);
    set_can_focus(true);
    set_focus_on_click(true);
    add_events(
        Gdk::KEY_PRESS_MASK | 
        Gdk::BUTTON_PRESS_MASK |
        Gdk::POINTER_MOTION_MASK);

    // Create the text layout
    layout = create_pango_layout("");
    layout->set_text(buffer);
    set_font("Monospace 12");

    // Create cursor
    cursor = new CodeViewCursor(layout, this);
    cursor->signal_updated([this]() { queue_draw(); });

    // Cursor blink event
    Glib::signal_timeout().connect(
        sigc::mem_fun(this, &CodeView::on_blink_update), 500);
}

CodeView::~CodeView()
{
    delete cursor;
}

void CodeView::set_font(string font_name)
{
    Pango::FontDescription font(font_name);
    layout->set_font_description(font);
    set_tab_size(tab_size);
}

void CodeView::set_tab_size(int size)
{
    // Find the width of each space
    int width, height;
    auto space_layout = create_pango_layout(" ");
    space_layout->set_font_description(layout->get_font_description());
    space_layout->get_pixel_size(width, height);

    // Set the tab sizing
    Pango::TabArray tabs(1, true);
    tabs.set_tab(0, Pango::TAB_LEFT, width * size);
    layout->set_tabs(tabs);
    tab_size = size;
}

void CodeView::set_buffer(string str)
{
    layout->set_text(str);
    buffer = str;
}

bool CodeView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    const auto allocation = get_allocation();
    auto style_context = get_style_context();

    // Draw the background
    style_context->render_background(cr, 
        allocation.get_x(), allocation.get_y(),
        allocation.get_width(), allocation.get_height());

    // Draw content
    draw_buffer(cr);
    if (!cursor->is_in_blink())
        draw_cursor(cr, cursor);
}

void CodeView::draw_buffer(const Cairo::RefPtr<Cairo::Context>& cr)
{
//    auto tabs = Pango::TabArray(1, false);
//    tabs.set_tab(0, Pango::TAB_LEFT, font.get_size() * tab_size);
//    layout->set_tabs(tabs);

    int text_width;
    int text_height;
    layout->get_pixel_size(text_width, text_height);
    set_size_request(text_width, text_height);

    Pango::AttrList attrs;
    cursor->apply_styles(get_style_context(), attrs);
    layout->set_attributes(attrs);

    cr->set_source_rgb(1, 1, 1);
    cr->move_to(0, 0);
    layout->show_in_cairo_context(cr);
}

void CodeView::draw_cursor(const Cairo::RefPtr<Cairo::Context>& cr,
    CodeViewCursor *cursor)
{
    // Draw the cursor on screen
    auto pos = layout->get_cursor_weak_pos(cursor->get_index());
    int x = pos.get_x() / 1024;
    int y = pos.get_y() / 1024;
    int h = pos.get_height() / 1024;
    cr->set_source_rgb(1, 1, 1);
    cr->move_to(x, y);
    cr->line_to(x, y + h);
    cr->stroke();

    // Update the scroll positions
    
}

void CodeView::on_realize()
{
    set_realized();

    if (!window)
    {
        GdkWindowAttr attributes;
        Gtk::Allocation allocation = get_allocation();
        memset(&attributes, 0, sizeof(attributes));

        //Set initial position and size of the Gdk::Window:
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();

        attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;

        window = Gdk::Window::create(get_parent_window(), &attributes,
            GDK_WA_X | GDK_WA_Y);
        set_window(window);

        //make the widget receive expose events
        window->set_user_data(gobj());
    }
}

bool CodeView::on_button_press_event(GdkEventButton* button_event)
{
    grab_focus();
    return Gtk::Widget::on_button_press_event(button_event);
}

void CodeView::on_unrealize()
{
    window.reset();

    // Call base class
    Gtk::Widget::on_unrealize();
}

bool CodeView::on_blink_update()
{
    cursor->update_blink();
    queue_draw();
    return true;
}
