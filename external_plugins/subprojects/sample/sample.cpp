#include <iostream>
#include "sample.h"
#include <stringMatch.h>

std::string SampleMatch::getDisplay() const
{
    return value;
}

RunResult SampleMatch::run()
{
    std::cout << "Running Sample Match with value: " << value << std::endl;
    return RunResult::CLOSE;
}

double SampleMatch::getRelevance(const std::string &input) const
{
    return fuzzyMatchScore(input, value);
}

std::vector<Match *> SamplePlugin::getMatches(const std::string &input) const
{
    // get options array from pluginSettings
    std::vector<Match *> matches;
    if (pluginSettings.find("options") == pluginSettings.end())
    {
        std::cerr << "No options found in plugin settings." << std::endl;
    }
    else
    {
        for (const auto &item : pluginSettings["options"])
        {
            std::string value = item.get<std::string>();
            if (hasAllChars(input, value))
            {
                matches.push_back(new SampleMatch(value));
            }
        }
    }
    return matches;
}

extern "C" Plugin *create()
{
    return new SamplePlugin();
}