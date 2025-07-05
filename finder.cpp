// Local Headers
#include <finder.h>
#include <ai.h>
#include <dmenu.h>
#include <drun.h>
#include <equation.h>
#include <run.h>
#include <web.h>
#include <pluginLoader.h>

Finder::Finder(const std::vector<std::string> &modes, const std::map<std::string, std::map<std::string, std::string>> &pluginOptions)
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

    std::string systemPluginPath = std::getenv("GORG_SYSTEM_PLUGIN_PATH") ? std::getenv("GORG_SYSTEM_PLUGIN_PATH") : "/usr/local/share/gorg/plugins";
    auto systemPlugins = loadPluginDirectory(systemPluginPath);
    allPlugins.insert(allPlugins.end(), systemPlugins.begin(), systemPlugins.end());

    // If modes are specified, only load those plugins
    if (!modes.empty())
    {
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

        // Delete unused plugins
        for (auto plugin : allPlugins)
        {
            if (std::find(plugins.begin(), plugins.end(), plugin) == plugins.end())
            {
                delete plugin;
            }
        }
    }
    else
    {
        // No modes specified, load default plugins
        plugins = allPlugins;
    }

    // Load settings with any command line overrides
    loadPluginSettings(pluginOptions);
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

void Finder::loadPluginSettings(const std::map<std::string, std::map<std::string, std::string>> &pluginOptions)
{
    for (auto plugin : plugins)
    {
        nlohmann::json settings = getPluginSettings(plugin->getName());

        // Apply command line overrides if they exist for this plugin
        auto pluginIt = pluginOptions.find(plugin->getName());
        if (pluginIt != pluginOptions.end())
        {
            for (const auto &[key, value] : pluginIt->second)
            {
                // Try to parse the value as a number if possible
                try
                {
                    if (value.find('.') != std::string::npos)
                    {
                        // Try as double
                        double doubleValue = std::stod(value);
                        settings[key] = doubleValue;
                    }
                    else
                    {
                        // Try as integer
                        int intValue = std::stoi(value);
                        settings[key] = intValue;
                    }
                }
                catch (const std::exception &)
                {
                    // Not a number, treat as string
                    settings[key] = value;
                }
            }
        }

        plugin->setSettings(settings);
    }
}
