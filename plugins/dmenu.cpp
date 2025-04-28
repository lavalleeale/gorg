// C++ Standard Library
#include <iostream>

// Project Headers
#include <dmenu.h>
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
    for (size_t i = 0; i < options.size(); i++)
    {
        if (hasAllChars(input, options[i]))
        {
            matches.push_back(new DmenuMatch(options[i], pluginSettings.value("relevance", 1) * static_cast<double>(options.size() - i) / options.size()));
        }
    }
    return matches;
}