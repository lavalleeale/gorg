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
    std::vector<Match *> matches;
    for (const auto &item : cache)
    {
        if (hasAllChars(input, item->getDisplay()))
        {
            matches.push_back(item.get());
        }
    }
    return matches;
}

void SamplePlugin::setSettings(const nlohmann::json &settings)
{
    pluginSettings = settings;
    cache.clear();
    if (pluginSettings.find("options") == pluginSettings.end())
    {
        std::cerr << "No options found in plugin settings." << std::endl;
    }
    else
    {
        for (const auto &item : pluginSettings["options"])
        {
            std::string value = item.get<std::string>();
            cache.push_back(std::make_unique<SampleMatch>(value));
        }
    }
}

extern "C" Plugin *create()
{
    return new SamplePlugin();
}

extern "C" int gorg_plugin_api_version()
{
    return GORG_PLUGIN_API_VERSION;
}
