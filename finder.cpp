#include "finder.h"
#include "plugins/drun.h"
#include "plugins/equation.h"
#include "plugins/chat.h"
#include "plugins/run.h"

Finder::Finder()
{
    plugins.push_back(new Drun());
    plugins.push_back(new Equation());
    plugins.push_back(new Chat());
    plugins.push_back(new Run());
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
        if (plugin->matchChance(query) > 0)
        {
            std::vector<Match *> pluginMatches = plugin->getMatches(query);
            matches.insert(matches.end(), pluginMatches.begin(), pluginMatches.end());
        }
    }
    std::sort(matches.begin(), matches.end(), [&query](Match *a, Match *b)
              { return a->getRelevance(query) > b->getRelevance(query); });
}
