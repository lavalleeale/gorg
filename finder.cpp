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
    matches.clear();

    // parse optional "!pluginName" prefix
    std::string q = query;
    std::string filter;
    if (!q.empty() && q[0] == '!')
    {
        size_t pos = q.find(' ');
        if (pos != std::string::npos)
        {
            filter = q.substr(1, pos - 1);
            q = q.substr(pos + 1);
        }
        else
        {
            filter = q.substr(1);
            q.clear();
        }
    }

    unsigned int found = 0;
    for (auto plugin : plugins)
    {
        if (!plugin->getName().starts_with(filter))
            continue;
        found++;
    }
    for (auto plugin : plugins)
    {
        if (found && !plugin->getName().starts_with(filter))
            continue;
        auto pluginMatches = plugin->getMatches(q);
        matches.insert(matches.end(), pluginMatches.begin(), pluginMatches.end());
        if ((--found == 0) || matches.size() >= getMaxResults())
        {
            break; // stop searching if we found a match or reached max results
        }
    }

    // sort by relevance against stripped query
    std::sort(matches.begin(), matches.end(), [&q](Match *a, Match *b)
              { return a->getRelevance(q) > b->getRelevance(q); });
}

void Finder::loadPluginSettings()
{
    for (auto plugin : plugins)
    {
        plugin->setSettings(getPluginSettings(plugin->getName()));
    }
}
