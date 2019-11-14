#pragma once
#include <rapidjson/document.h>
#include <string>
using std::string;

namespace lc
{

    class Options
    {
    public:
        static void load();
        static void save();

    private:
        static void find_options_dir();
        static string options_dir;

    };

}
