// C++ Standard Library
#include <iostream>

// Project Headers
#include <dmenu.h>
#include <stringMatch.h>

double DmenuMatch::getRelevance(const std::string &) const
{
    return relevance;
}

std::string DmenuMatch::getDisplay() const
{
    return value;
}

RunResult DmenuMatch::run()
{
    std::cout << value << std::endl;
    return RunResult::CLOSE;
}

std::vector<Match *> Dmenu::getMatches(const std::string &input) const
{
    std::vector<Match *> matches;
    for (const auto &item : cache)
    {
        if (hasAllChars(input, item->getDisplay()))
        {
            matches.push_back(new DmenuMatch(item->getDisplay(), item->getRelevance(input)));
        }
    }
    return matches;
}

Dmenu::~Dmenu()
{
    for (auto &item : cache)
    {
        delete item;
    }
    cache.clear();
}

void Dmenu::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
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
        cache.push_back(new DmenuMatch(line, pluginSettings.value("relevance", 1)));
    }
}