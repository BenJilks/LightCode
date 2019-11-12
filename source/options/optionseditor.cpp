#include "options/optionseditor.hpp"
#include <gtkmm/builder.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/fontchooserdialog.h>
using namespace lc;

pair<string, int> OptionsEditor::font;
void OptionsEditor::load()
{
    font = std::make_pair("", -1);
}

void OptionsEditor::font_change()
{
    auto font_map = font_button->get_font_name();
    font.first = font_map;
}

OptionsEditor::OptionsEditor()
{
    set_default_size(200, 200);
    set_title("Options - Editor");

    auto ui = Gtk::Builder::create();
    ui->add_from_file("../test.glade");

    Gtk::Box *cont;
    ui->get_widget("cont", cont);
    ui->get_widget("font", font_button);

    font_button->set_use_font(true);
    font_button->signal_font_set().connect(
        sigc::mem_fun(*this, &OptionsEditor::font_change));
    get_content_area()->add(*cont);
    show_all_children();
}
