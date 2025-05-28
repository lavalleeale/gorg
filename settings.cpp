// C++ Standard Library
#include <fstream>
#include <iostream>
#include <mutex>

// Third Party Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <settings.h>

static int windowWidth = 600;
static int windowHeight = 200;
static unsigned int maxResults = 25;
static float imageSize = 32.0f; // Default image size
static std::string customCss;
static std::string lastQuery;
static nlohmann::json pluginSettings;
static std::once_flag loaded;

void loadFromDir(const std::string &directory)
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
        file.close();

        windowWidth = j.value("windowWidth", 600);
        windowHeight = j.value("windowHeight", 200);
        maxResults = j.value("maxResults", 25);
        imageSize = j.value("imageSize", 32.0f);
        lastQuery = j.value("lastQuery", "");
        pluginSettings = j.value("plugins", nlohmann::json::object());
        std::ifstream customCssFile(directory + "/style.css");
        if (customCssFile.is_open())
        {
            customCssFile.seekg(0, std::ios::end);
            customCss.reserve(customCssFile.tellg());
            customCssFile.seekg(0, std::ios::beg);

            customCss.assign((std::istreambuf_iterator<char>(customCssFile)),
                             std::istreambuf_iterator<char>());
            customCssFile.close();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing settings file: " << e.what() << std::endl;
    }
}

void load()
{
    loadFromDir(getConfDir());
}

nlohmann::json getPluginSettings(const std::string &pluginName)
{
    std::call_once(loaded, []()
                   { load(); });
    static const nlohmann::json empty = nlohmann::json::object();
    auto it = pluginSettings.find(pluginName);
    return it != pluginSettings.end() ? *it : empty;
}

int getWindowWidth()
{
    std::call_once(loaded, []()
                   { load(); });
    return windowWidth;
}

int getWindowHeight()
{
    std::call_once(loaded, []()
                   { load(); });
    return windowHeight;
}

unsigned int getMaxResults()
{
    std::call_once(loaded, []()
                   { load(); });
    return maxResults;
}

std::string getCustomCss()
{
    std::call_once(loaded, []()
                   { load(); });
    return customCss;
}

std::string getLastQuery()
{
    std::call_once(loaded, []()
                   { load(); });
    return lastQuery;
}

std::string getConfDir()
{
    char *xdgConfigHome = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome)
    {
        return std::string(xdgConfigHome) + "/gorg";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            return std::string(home) + "/.config/gorg";
        }
        else
        {
            std::cerr << "Could not find XDG_CONFIG_HOME or HOME environment variables" << std::endl;
            return "";
        }
    }
}

float getImageSize()
{
    std::call_once(loaded, []()
                   { load(); });
    return imageSize;
}

void saveLastQuery(const std::string &query)
{
    char *xdgConfigHome = std::getenv("XDG_CONFIG_HOME");
    std::string configPath;

    if (xdgConfigHome)
    {
        configPath = std::string(xdgConfigHome) + "/gorg/config.json";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            configPath = std::string(home) + "/.config/gorg/config.json";
        }
        else
        {
            std::cerr << "Could not find XDG_CONFIG_HOME or HOME environment variables" << std::endl;
            return;
        }
    }

    try
    {
        std::ifstream file(configPath);
        nlohmann::json j;

        if (file.is_open())
        {
            file >> j;
            file.close();
        }

        j["lastQuery"] = query;

        std::ofstream outFile(configPath);
        outFile << j.dump(4);
        outFile.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error saving last query: " << e.what() << std::endl;
    }
}