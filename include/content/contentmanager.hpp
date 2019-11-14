#pragma once
#include "contentpage.hpp"
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace lc
{

    class ContentManager;
    class PageTitle : public Gtk::Box
    {
    public:
        PageTitle(ContentManager *manager, ContentPage *page);
        void update_title();

    private:
        void exit_clicked();

        Gtk::Label label, exit_label;
        Gtk::Button exit;
        ContentPage *page;
        ContentManager *manager;

    };

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
        vector<PageTitle*> page_titles;
        vector<ContentPage*> pages;

    };

}
