#ifndef __dmenu_h__
#define __dmenu_h__

// Third Party Libraries
#include <nlohmann/json.hpp>

// Local Headers
#include <plugin.h>
#include <match.h>
#include <settings.h>

class DmenuMatch : public Match
{
public:
    DmenuMatch(const std::string &value, double relevance) : value(value), relevance(relevance) {};
    std::string getDisplay() const override;
    RunResult run() override;
    double getRelevance(const std::string &input) const override;
    ~DmenuMatch() override {};

private:
    std::string value;
    nlohmann::json pluginSettings = getPluginSettings("dmenu");
    double relevance;
};
class Dmenu : public Plugin
{
public:
    Dmenu();
    std::vector<Match *> getMatches(const std::string &input) const override;
    ~Dmenu() override {};

private:
    std::vector<std::string> options;
};
#endif