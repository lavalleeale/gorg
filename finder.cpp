// Local Headers
#include <finder.h>
#include <chat.h>
#include <dmenu.h>
#include <drun.h>
#include <equation.h>
#include <run.h>
#include <web.h>

Finder::Finder()
{
    plugins.push_back(new Drun());
    plugins.push_back(new Equation());
    plugins.push_back(new Chat());
    plugins.push_back(new Run());
    plugins.push_back(new Web());
}

Finder::Finder(const std::vector<std::string> &modes)
{
    std::string lowercase;
    for (const auto &mode : modes)
    {
        lowercase = mode;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);
        if (lowercase == "drun")
        {
            plugins.push_back(new Drun());
        }
        else if (lowercase == "equation")
        {
            plugins.push_back(new Equation());
        }
        else if (lowercase == "chat")
        {
            plugins.push_back(new Chat());
        }
        else if (lowercase == "run")
        {
            plugins.push_back(new Run());
        }
        else if (lowercase == "web")
        {
            plugins.push_back(new Web());
        }
        else if (lowercase == "dmenu")
        {
            plugins.push_back(new Dmenu());
        }
    }
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
