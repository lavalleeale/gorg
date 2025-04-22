#ifndef __run_h__
#define __run_h__
#include "plugin.h"
#include "match.h"
#include <filesystem>
#include <gtkmm/widget.h>
#include <nlohmann/json.hpp>
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