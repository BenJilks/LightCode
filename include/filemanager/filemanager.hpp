#pragma once
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <string>
#include <functional>
using std::string;

namespace lc
{

    class FileManager : public Gtk::ScrolledWindow
    {
    public:
        FileManager();

        inline void signal_file_open(std::function<void(string)> callback)
            { file_open_callback = callback; }
        void open_folder(string path);

    protected:
        void open_sub_dir(string path, Gtk::TreeModel::Row *row);
        void item_selected(const Gtk::TreeModel::Path& path,
            Gtk::TreeViewColumn*);

        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
            ModelColumns() 
                { add(col_name); add(col_path); add(col_is_file); }
            
            Gtk::TreeModelColumn<string> col_name;
            Gtk::TreeModelColumn<string> col_path;
            Gtk::TreeModelColumn<bool> col_is_file;
        };

        ModelColumns columns;
        Gtk::TreeView tree_view;
        Glib::RefPtr<Gtk::TreeStore> tree_model;
        std::function<void(string)> file_open_callback;

    };

}
