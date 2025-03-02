#include <settings.h>
#include <fstream>
#include <iostream>

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

void Settings::loadFromDir(const std::string &directory)
{
    try
    {
        std::ifstream file(directory + "/config.json");
        if (!file.is_open())
        {
            std::cerr << "Could not open settings file: " << directory + "/config.json" << std::endl;
            return;
        }

        nlohmann::json j;
        file >> j;

        windowWidth = j.value("windowWidth", 600);
        windowHeight = j.value("windowHeight", 200);
        maxResults = j.value("maxResults", 25);
        pluginSettings = j.value("plugins", nlohmann::json::object());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing settings file: " << e.what() << std::endl;
    }
}

void Settings::load()
{
    char *xdgConfigHome = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome)
    {
        loadFromDir(std::string(xdgConfigHome) + "/gorg");
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            loadFromDir(std::string(home) + "/.config/gorg");
        }
        else
        {
            std::cerr << "Could not find XDG_CONFIG_HOME or HOME environment variables" << std::endl;
        }
    }
}

const nlohmann::json &Settings::getPluginSettings(const std::string &pluginName) const
{
    static const nlohmann::json empty = nlohmann::json::object();
    auto it = pluginSettings.find(pluginName);
    return it != pluginSettings.end() ? *it : empty;
}
