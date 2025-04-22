#ifndef __dmenu_h__
#define __dmenu_h__
#include "plugin.h"
#include "match.h"
#include <nlohmann/json.hpp>
#include "settings.h"

class DmenuMatch : public Match
{
public:
    DmenuMatch(std::string value, double relevance) : value(value), pluginSettings(getPluginSettings("dmenu")), relevance(relevance) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~DmenuMatch() {};

private:
    std::string value;
    nlohmann::json pluginSettings;
    double relevance;
};
class Dmenu : public Plugin
{
public:
    Dmenu();
    std::vector<Match *> getMatches(std::string input);
    ~Dmenu() {};

private:
    std::vector<std::string> options;
};
#endif