#include <finder.h>
#include <drun.h>
#include <equation.h>
#include <chat.h>
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
        std::vector<Match *> pluginMatches = plugin->getMatches(query);
        matches.insert(matches.end(), pluginMatches.begin(), pluginMatches.end());
    }
    std::sort(matches.begin(), matches.end(), [&query](Match *a, Match *b)
              { return a->getRelevance(query) > b->getRelevance(query); });
}
