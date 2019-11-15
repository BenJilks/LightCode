#include "filemanager/filemanager.hpp"
#include <filesystem>
using namespace lc;
namespace fs = std::filesystem;

FileManager::FileManager()
{
    tree_model = Gtk::TreeStore::create(columns);
    tree_view.set_model(tree_model);

    open_folder("/home/benjilks/pain/LightCode/");

    tree_view.append_column("File Name", columns.col_name);
    add(tree_view);
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
        
        if (entry.is_directory())
            open_sub_dir(full_path, &row);
    }
}
