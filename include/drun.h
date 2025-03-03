#ifndef __drun_h__
#define __drun_h__
#include "plugin.h"
#include "match.h"
#include <filesystem>
#include <gtkmm/widget.h>
#include <nlohmann/json.hpp>
#include <settings.h>
class DrunMatch : public Match
{
public:
    DrunMatch(std::filesystem::path path) : path(path), pluginSettings(Settings::getInstance().getPluginSettings("drun")) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~DrunMatch() {};

private:
    std::filesystem::path path;
    nlohmann::json pluginSettings;
};
class Drun : public Plugin
{
public:
    std::vector<Match *> getMatches(std::string input);
    ~Drun() {};
};
#endif