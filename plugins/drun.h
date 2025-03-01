#ifndef __drun_h__
#define __drun_h__
#include "plugin.h"
#include "match.h"
#include <filesystem>
#include <gtkmm/widget.h>
class DrunMatch : public Match
{
public:
    DrunMatch(std::filesystem::path path) : path(path) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~DrunMatch() {};

private:
    std::filesystem::path path;
};
class Drun : public Plugin
{
public:
    float matchChance(std::string input);
    std::vector<Match *> getMatches(std::string input);
    ~Drun() {};
};
#endif