#pragma once
#include "contentpage.hpp"
#include <gtkmm/notebook.h>
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace lc
{

    class ContentManager : public Gtk::Notebook
    {
    public:
        ContentManager();

        void add_page(ContentPage *page);
        ContentPage *current();
    
    private:
        vector<Gtk::Label> page_labels;
        vector<ContentPage*> pages;

    };

}
