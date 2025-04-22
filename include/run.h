#ifndef __run_h__
#define __run_h__

// External Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

class RunMatch : public Match
{
public:
    RunMatch(const std::string &exec, const std::string &name) : exec(exec), name(name) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~RunMatch() override {};

private:
    std::string exec;
    std::string name;
    nlohmann::json pluginSettings = getPluginSettings("run");
};
class Run : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    ~Run() override {};
};
#endif