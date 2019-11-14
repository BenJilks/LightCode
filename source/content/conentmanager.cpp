#include "content/contentmanager.hpp"
using namespace lc;

PageTitle::PageTitle(ContentManager *manager, ContentPage *page) :
    Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL),
    label(page->get_title()), exit_label("X"),
    page(page), manager(manager)
{
    exit.add(exit_label);
    exit.signal_clicked().connect(
        sigc::mem_fun(this, &PageTitle::exit_clicked));
    set_spacing(10);
    
    pack_start(label);
    pack_start(exit);
    show_all_children();
}

void PageTitle::update_title()
{
    label.set_text(page->get_title());
}

void PageTitle::exit_clicked()
{
    manager->close_page(page);
}

ContentManager::ContentManager()
{
    set_scrollable(true);
}

ContentManager::~ContentManager()
{
    for (auto page : pages)
        delete page;

    for (auto title : page_titles)
        delete title;
}

void ContentManager::add_page(ContentPage *page)
{
    PageTitle *title = new PageTitle(this, page);
    append_page(*page, *title);
    show_all_children();

    page_titles.push_back(title);
    pages.push_back(page);
}

void ContentManager::close_page(ContentPage *page)
{
    for (int i = 0; i < pages.size(); i++)
    {
        if (pages[i] == page)
        {
            pages.erase(pages.begin() + i);
            remove_page(*page);
            delete page;
        }
    }
}

void ContentManager::apply_settings()
{
    for (auto page : pages)
        page->apply_settings();
}

ContentPage *ContentManager::current()
{
    int index = get_current_page();
    return (ContentPage*)get_nth_page(index);
}
