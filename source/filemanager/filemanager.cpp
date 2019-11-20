#include "filemanager/filemanager.hpp"
#include <filesystem>
using namespace lc;
namespace fs = std::filesystem;

FileManager::FileManager()
{
    tree_model = Gtk::TreeStore::create(columns);
    tree_view.set_model(tree_model);
    tree_view.signal_row_activated().connect(
        sigc::mem_fun(this, &FileManager::item_selected));
    open_folder("/home/benjilks/Dropbox/school/webbased/assignment2/WebAssignment2/");

    tree_view.append_column("File Name", columns.col_name);
    add(tree_view);
}

void FileManager::item_selected(const Gtk::TreeModel::Path& path,
    Gtk::TreeViewColumn*)
{
    Gtk::TreeModel::iterator iter = tree_model->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        if (row[columns.col_is_file])
            file_open_callback(row[columns.col_path]);
        else
        {
            if (!tree_view.row_expanded(path))
                tree_view.expand_row(path, false);
            else
                tree_view.collapse_row(path);
        }
    }
}

void FileManager::open_folder(string path)
{
    open_sub_dir(path, nullptr);
}

void FileManager::open_sub_dir(string path, Gtk::TreeModel::Row *parent)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        string full_path = entry.path();
        int name_start = full_path.find_last_of('/') + 1;
        string name = full_path.substr(name_start, 
            full_path.length() - name_start);

        // Don't show hidden files
        if (name[0] == '.')
            continue;

        Gtk::TreeRow row;
        if (parent == nullptr)
            row = *(tree_model->append());
        else
            row = *(tree_model->append(parent->children()));
        row[columns.col_name] = name;
        row[columns.col_path] = full_path;
        row[columns.col_is_file] = !entry.is_directory();
        
        if (entry.is_directory())
            open_sub_dir(full_path, &row);
    }
}
