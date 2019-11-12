#include "window.hpp"
#include "content/textedit.hpp"
#include "options/optionseditor.hpp"
#include <gtkmm.h>
using namespace lc;

static const char *menu_bar_layout = 
	"<ui>"
	"	<menubar name='MenuBar'>"
	"		<menu action='FileMenu'>"
	"			<menuitem action='New' accel='&lt;Primary&gt;n' />"
	"			<menuitem action='Open' accel='&lt;Primary&gt;o' />"
	"			<menuitem action='Save' />"
	"			<menuitem action='SaveAs' />"
	"		</menu>"
	"		<menu action='OptionsMenu'>"
	"			<menuitem action='Editor' />"
	"		</menu>"
	"	</menubar>"
	"</ui>";

void Window::on_file_new()
{
	// Clear the editor of its current content, 
	// including reference to files
	content.add_page(new TextEdit());
}

void Window::on_file_open()
{
	Gtk::FileChooserDialog file_chooser("Please choose a file");
	file_chooser.add_button("_Select", Gtk::RESPONSE_OK);
	file_chooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	
	int result = file_chooser.run();
	if (result == Gtk::RESPONSE_OK)
	{
		string file_path = file_chooser.get_filename();
		TextEdit *edit_page = new TextEdit(file_path);

		ContentPage *curr_page = content.current();
		if (curr_page->has_content())
			content.add_page(edit_page);
		else
			curr_page->open(file_path);
	}
}

void Window::on_file_save()
{

}

void Window::on_file_save_as()
{

}

void Window::options_editor()
{
	OptionsEditor options;
	options.run();
}

Window::Window() :
	layout(Gtk::Orientation::ORIENTATION_VERTICAL)
{
	// Create menubar 
	auto action_group = Gtk::ActionGroup::create();
	action_group->add(Gtk::Action::create("FileMenu", "_File"));
	action_group->add(Gtk::Action::create("OptionsMenu", "_Options"));

	// Create actions
	action_group->add(Gtk::Action::create("New", "_New"), 
		sigc::mem_fun(*this, &Window::on_file_new));
	action_group->add(Gtk::Action::create("Open", "_Open"),
		sigc::mem_fun(*this, &Window::on_file_open));
	action_group->add(Gtk::Action::create("Save", "_Save"),
		sigc::mem_fun(*this, &Window::on_file_save));
	action_group->add(Gtk::Action::create("SaveAs", "Save _As"),
		sigc::mem_fun(*this, &Window::on_file_save_as));
	
	action_group->add(Gtk::Action::create("Editor", "_Editor"), 
		sigc::mem_fun(*this, &Window::options_editor));

	// Create menu bar ui
	auto ui = Gtk::UIManager::create();
	ui->insert_action_group(action_group);
	ui->add_ui_from_string(menu_bar_layout);

	// Add menu bar to window
	add_accel_group(ui->get_accel_group());
	layout.pack_start(*ui->get_widget("/MenuBar"), 
		Gtk::PackOptions::PACK_SHRINK);

	// Create content
	content.add_page(new TextEdit());
	layout.pack_start(content);

	add(layout);
	set_default_size(800, 600);
	show_all_children();
}
