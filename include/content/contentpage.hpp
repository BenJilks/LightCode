#pragma once
#include "settings/settings.hpp"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <string>
#include <functional>
#include <map>
using std::string;
using std::map;

namespace lc
{

    class ContentTitle : public Gtk::Box
    {
    public:
        ContentTitle(string title);

        inline auto signal_exit() { return exit.signal_clicked(); }
        void update_title(string title);

    private:
        void exit_clicked();
        Gtk::Label label, exit_label;
        Gtk::Button exit;

    };

    class SettingsManager;
    class ContentPage : public Gtk::ScrolledWindow
    {
    public:
        ContentPage();
        
        void signal_exit(std::function<void()> callback) { exit_callback = callback; }
        void signal_apply(std::function<void()> callback) { apply_callback = callback; }
        inline ContentTitle &get_title_widget() { return title; }
        virtual void open(string file_path) {}

        virtual string get_title() const = 0;
        virtual bool has_content() const = 0;
        virtual void apply_settings(SettingsManager *settings) = 0;
    
    protected:
        void on_exit();

        ContentTitle title;
        std::function<void()> exit_callback;
        std::function<void()> apply_callback;

    };

}
