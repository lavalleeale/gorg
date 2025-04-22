#ifndef __web_h__
#define __web_h__

// External Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <match.h>
#include <plugin.h>
#include <settings.h>

class WebMatch : public Match
{
public:
    WebMatch(const std::string &input) : input(input) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~WebMatch() override {};

private:
    std::string input;
    nlohmann::json pluginSettings = getPluginSettings("web");
};
class Web : public Plugin
{
public:
    std::vector<Match *> getMatches(const std::string &input) const override;
    ~Web() override {};

private:
};
#endif