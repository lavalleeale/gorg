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
    DrunMatch(const std::filesystem::path &path, double relevance) : path(path), relevance(relevance) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~DrunMatch() override {};

private:
    std::filesystem::path path;
    double relevance;
};
class Drun : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    std::string getName() const override { return "drun"; };
    void setSettings(const nlohmann::json &settings) override;
    ~Drun() override {};

private:
    std::vector<DrunMatch *> cache;
};
#endif