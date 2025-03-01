#ifndef __run_h__
#define __run_h__
#include "plugin.h"
#include "match.h"
#include <filesystem>
#include <gtkmm/widget.h>
class RunMatch : public Match
{
public:
    RunMatch(std::string exec, std::string name) : exec(exec), name(name) {};
    std::string getDisplay();
    RunResult run();
    double getRelevance(std::string input);
    Gtk::Widget *getWidget() { return nullptr; };
    ~RunMatch() {};

private:
    std::string exec;
    std::string name;
};
class Run : public Plugin
{
public:
    float matchChance(std::string input);
    std::vector<Match *> getMatches(std::string input);
    ~Run() {};
};
#endif