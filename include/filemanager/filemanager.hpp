#pragma once
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <string>
using std::string;

namespace lc
{

    class FileManager : public Gtk::ScrolledWindow
    {
    public:
        FileManager();
        void open_folder(string path);

    protected:
        void open_sub_dir(string path, Gtk::TreeModel::Row *row);

        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
            ModelColumns() 
                { add(col_name); }
            
            Gtk::TreeModelColumn<string> col_name;
        };

        ModelColumns columns;
        Gtk::TreeView tree_view;
        Glib::RefPtr<Gtk::TreeStore> tree_model;

    };

}
