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
    RunMatch(std::string exec, std::string name) : exec(exec), name(name), pluginSettings(Settings::getInstance().getPluginSettings("run")) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~RunMatch() {};

private:
    std::string exec;
    std::string name;
    nlohmann::json pluginSettings;
};
class Run : public Plugin
{
public:
    std::vector<Match *> getMatches(std::string input);
    ~Run() {};
};
#endif