#ifndef __web_h__
#define __web_h__
#include "plugin.h"
#include <nlohmann/json.hpp>
#include "match.h"
#include <gtkmm/label.h>
#include <glibmm/dispatcher.h>
#include <settings.h>
class WebMatch : public Match
{
public:
    WebMatch(std::string input) : input(input), pluginSettings(getPluginSettings("web")) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~WebMatch() {};

private:
    nlohmann::json pluginSettings;
    std::string input;
};
class Web : public Plugin
{
public:
    std::vector<Match *> getMatches(std::string input);
    ~Web() {};

private:
};
#endif