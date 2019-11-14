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
        ~ContentManager();

        void add_page(ContentPage *page);
        void close_page(ContentPage *page);
        void apply_settings();
        ContentPage *current();
    
    private:
        vector<ContentPage*> pages;

    };

}
