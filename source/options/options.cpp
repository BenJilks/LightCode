#include "options/options.hpp"
#include "options/optionseditor.hpp"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include <pwd.h>
#include <sys/stat.h>
using namespace lc;

string Options::options_dir = "";

void Options::load()
{
    if (options_dir == "")
        find_options_dir();

    // Open the file and find its length
    std::ifstream ifstream(options_dir + "/config.json");
    if (ifstream.good())
    {
        rapidjson::IStreamWrapper stream(ifstream);

        // Parse the json stream
        rapidjson::Document settings_doc;
        settings_doc.ParseStream(stream);
        
        if (settings_doc.IsObject())
        {
            rapidjson::Value &editor = settings_doc["editor"];
            if (editor.IsObject()) OptionsEditor::load_settings(editor);
        }

        // Clean up
        ifstream.close();
    }
}

void Options::find_options_dir()
{
    // Find users config folder
    struct passwd *pw = getpwuid(getuid());
    options_dir = string(pw->pw_dir) + "/.config/lightcode";

    // Create folders if they do not exist already
    mkdir(options_dir.c_str(), 0777);
}

void Options::save()
{
    // Open the output stream
    std::ofstream ofstream(options_dir + "/config.json");
    rapidjson::OStreamWrapper stream(ofstream);
    rapidjson::Document settings_doc;

    // Write new json data
    auto &allocator = settings_doc.GetAllocator();
    auto editor = OptionsEditor::save_settings(allocator);
    settings_doc.SetObject();
    settings_doc.AddMember("editor", editor, allocator);

    // Write the file to stream
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(stream);
    settings_doc.Accept(writer);
    ofstream.close();
}
