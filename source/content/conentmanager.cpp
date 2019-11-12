#include "content/contentmanager.hpp"
using namespace lc;

ContentManager::ContentManager()
{
    set_scrollable(true);
}

void ContentManager::add_page(ContentPage *page)
{
    page_labels.emplace_back(page->get_title());
    pages.push_back(page);

    int last = pages.size() - 1;
    append_page(*page, page_labels[last]);
    show_all_children();
}

ContentPage *ContentManager::current()
{
    int index = get_current_page();
    return (ContentPage*)get_nth_page(index);
}
