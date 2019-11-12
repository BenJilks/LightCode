#pragma once
#include <gtkmm/scrolledwindow.h>
#include <string>
using std::string;

namespace lc
{

    class ContentPage : public Gtk::ScrolledWindow
    {
    public:
        ContentPage() {}

        virtual void open(string file_path) = 0;

        virtual string get_title() const = 0;
        virtual bool has_content() const = 0;

    private:


    };

}
