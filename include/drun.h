#ifndef __drun_h__
#define __drun_h__

// C++ Standard Library
#include <filesystem>

// External Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

class DrunMatch : public Match
{
public:
    DrunMatch(const std::filesystem::path &path) : path(path), pluginSettings(getPluginSettings("drun")) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~DrunMatch() override {};

private:
    std::filesystem::path path;
    nlohmann::json pluginSettings;
};
class Drun : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    ~Drun() override {};
};
#endif