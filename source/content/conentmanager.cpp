#include "content/contentmanager.hpp"
using namespace lc;

ContentManager::ContentManager()
{
    set_scrollable(true);
}

ContentManager::~ContentManager()
{
    for (auto page : pages)
        delete page;
}

void ContentManager::add_page(ContentPage *page)
{
    pages.push_back(page);
    page->signal_exit([this, page]() { close_page(page); });

    append_page(*page, page->get_title_widget());
    show_all_children();
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
