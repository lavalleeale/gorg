#include "dmenu.h"
#include <iostream>
#include <stringMatch.h>

Dmenu::Dmenu()
{
    if (isatty(STDIN_FILENO))
    {
        std::cerr << "Cannot read from stdin. Dmenu plugin is disabled." << std::endl;
        return;
    }
    // consume all of stdin and store each line in options
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
            break;
        options.push_back(line);
    }
    // remove duplicates
    for (auto it = options.begin(); it != options.end();)
    {
        if (std::count(options.begin(), options.end(), *it) > 1)
        {
            it = options.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

double DmenuMatch::getRelevance(std::string input)
{
    return pluginSettings.value("relevanceScore", 0.5) * relevance;
}

std::string DmenuMatch::getDisplay()
{
    return value;
}

RunResult DmenuMatch::run()
{
    std::cout << value << std::endl;
    return RunResult::CLOSE;
}

std::vector<Match *> Dmenu::getMatches(std::string input)
{
    std::vector<Match *> matches;
    for (size_t i = 0; i < options.size(); i++)
    {
        if (hasAllChars(input, options[i]))
        {
            matches.push_back(new DmenuMatch(options[i], static_cast<double>(options.size() - i) / options.size()));
        }
    }
    return matches;
}