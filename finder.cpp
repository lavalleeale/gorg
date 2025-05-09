// Local Headers
#include <finder.h>
#include <ai.h>
#include <dmenu.h>
#include <drun.h>
#include <equation.h>
#include <run.h>
#include <web.h>
#include <pluginLoader.h>

Finder::Finder()
{
    plugins.push_back(new Drun());
    plugins.push_back(new Equation());
    plugins.push_back(new AI());
    plugins.push_back(new Run());
    plugins.push_back(new Web());
    auto userPlugins = loadPluginDirectory(getConfDir() + "/plugins");
    plugins.insert(plugins.end(), userPlugins.begin(), userPlugins.end());
    std::string systemPluginPath = std::getenv("GORG_SYSTEM_PLUGIN_PATH") ? std::getenv("GORG_SYSTEM_PLUGIN_PATH") : "/usr/local/share/gorg/plugins";
    auto systemPlugins = loadPluginDirectory(systemPluginPath);
    plugins.insert(plugins.end(), systemPlugins.begin(), systemPlugins.end());
    loadPluginSettings();
}

Finder::Finder(const std::vector<std::string> &modes)
{
    std::vector<Plugin *> allPlugins = {
        new Drun(),
        new Equation(),
        new AI(),
        new Run(),
        new Web(),
        new Dmenu()};
    auto extPlugins = loadPluginDirectory(getConfDir() + "/plugins");
    allPlugins.insert(allPlugins.end(), extPlugins.begin(), extPlugins.end());
    for (const auto &mode : modes)
    {
        for (auto plugin : allPlugins)
        {
            if (plugin->getName() == mode)
            {
                if (std::find(plugins.begin(), plugins.end(), plugin) == plugins.end())
                {
                    plugins.push_back(plugin);
                }
                break;
            }
        }
    }
    for (auto plugin : allPlugins)
    {
        if (std::find(plugins.begin(), plugins.end(), plugin) == plugins.end())
        {
            delete plugin;
        }
    }
    loadPluginSettings();
}

Finder::~Finder()
{
    for (auto plugin : plugins)
    {
        delete plugin;
    }
    for (auto match : matches)
    {
        delete match;
    }
}

RunResult Finder::RunMatch()
{
    if (matches.empty())
    {
        return CONTINUE;
    }
    return matches[0]->run();
}

void Finder::find(const std::string &query)
{
    for (auto match : matches)
    {
        delete match;
    }
    matches.clear();
    for (auto plugin : plugins)
    {
        auto pluginMatches = plugin->getMatches(query);
        matches.insert(matches.end(), pluginMatches.begin(), pluginMatches.end());
    }
    std::sort(matches.begin(), matches.end(), [&query](Match *a, Match *b)
              { return a->getRelevance(query) > b->getRelevance(query); });
}

void Finder::loadPluginSettings()
{
    for (auto plugin : plugins)
    {
        plugin->setSettings(getPluginSettings(plugin->getName()));
    }
}
