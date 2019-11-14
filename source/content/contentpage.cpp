#include "content/contentpage.hpp"
using namespace lc;

ContentTitle::ContentTitle(string title) :
    Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL),
    label(title), exit_label("X")
{
    exit.add(exit_label);
    set_spacing(10);
    
    pack_start(label);
    pack_start(exit);
    show_all_children();
}

void ContentTitle::update_title(string title)
{
    label.set_text(title);
}

ContentPage::ContentPage() :
    title("")
{
    title.signal_exit().connect(
        sigc::mem_fun(this, &ContentPage::on_exit));
}

void ContentPage::on_exit()
{
    exit_callback();
}
