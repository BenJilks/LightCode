#include "content/contentmanager.hpp"
#include "settings/settingsmanager.hpp"
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

void ContentManager::add_page(ContentPage *page, SettingsManager *settings)
{
    pages.push_back(page);
    page->signal_exit([this, page]() { close_page(page); });
    page->signal_apply([this, settings]() 
    { 
        apply_settings(settings);
        settings->save();
    });
    page->apply_settings(settings);

    append_page(*page, page->get_title_widget());
    set_tab_reorderable(*page, true);
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

void ContentManager::apply_settings(SettingsManager *settings)
{
    for (auto page : pages)
        page->apply_settings(settings);
}

ContentPage *ContentManager::current()
{
    int index = get_current_page();
    return (ContentPage*)get_nth_page(index);
}
